// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringInstrumentFingeringGraph.h"

void UStringInstrumentFingeringGraph::Init(const int8 fingerCount, const int8 stringCount)
{
	mFingerCount = fingerCount;
	mStringCount = stringCount;

	mRoot = NewObject<UGraphNode>();

	FStringPosition position;
	position.Fret = -1;
	position.String = -1;
	mRoot->StringPosition = position;
	mRoot->FingerIndex = -1;
	mRoot->Note = nullptr;
	mRoot->Children.Empty();

	FFingerboardState fingerboardState;
	fingerboardState.ParentIndex = -1;
	fingerboardState.ParentScore = 0;
	for (int iFinger = 0; iFinger < mFingerCount; iFinger++)
	{
		FFingerState fingerState;
		fingerState.Fret = 0;
		for (int iString = 0; iString < mStringCount; iString++)
		{
			fingerState.IsPinPressing.Add(false);
			fingerState.PinPressEndTick.Add(0);
		}

		fingerboardState.FingerStates.Add(fingerState);
	}

	mRoot->FingerboardStates.Add(fingerboardState);

	mLastLayer.Add(mRoot);
}

void UStringInstrumentFingeringGraph::AddNote(USongNote *note, const TArray<FStringPosition> &notePositions)
{
	TArray<UGraphNode*> currentLayer;

	for (int8 iFinger = 0; iFinger < mFingerCount; iFinger++)
	{
		for (int8 iPosition = 0; iPosition < notePositions.Num(); iPosition++)
		{
			UGraphNode *node = NewObject<UGraphNode>();
			node->StringPosition = notePositions[iPosition];
			node->Note = note;
			node->FingerboardStates.Empty();
			node->Parents.Empty();
			node->Children.Empty();

			if (notePositions[iPosition].Fret != 0)
			{
				node->FingerIndex = iFinger;
			}
			else
			{
				node->FingerIndex = -1;
			}

			for (UGraphNode *parentNode : mLastLayer)
			{
				parentNode->Children.Add(node);
			}

			currentLayer.Add(node);
		}
	}

	mLastLayer = currentLayer;
}

void UStringInstrumentFingeringGraph::CalculateFingering()
{
	mLastLayer.Empty();
	mLastLayer.Add(mRoot);
	TArray<UGraphNode*> currentLayer = mRoot->Children;

	while (currentLayer.Num() != 0)
	{
		for (UGraphNode *parentNode : mLastLayer)
		{
			UpdateFingerboardStates(parentNode, currentLayer[0]->Note);
			CalculateTransitionScores(parentNode);
		}

		mLastLayer = currentLayer;
		currentLayer = currentLayer[0]->Children;
	}

	UGraphNode *bestNode = nullptr;
	int32 bestScore = MAXINT32;

	for (UGraphNode *endNode : mLastLayer)
	{
		for (auto fingerboardStateIt = endNode->FingerboardStates.CreateIterator(); fingerboardStateIt; ++fingerboardStateIt)
		{
			if (fingerboardStateIt->ParentScore < bestScore)
			{
				bestNode = endNode;
				bestScore = fingerboardStateIt->ParentScore;
			}
		}
	}

	BuildOptimalFingering(bestNode);

	mRoot = nullptr;
	mLastLayer.Empty();
}

void UStringInstrumentFingeringGraph::UpdateFingerboardStates(UGraphNode *node, const USongNote* note)
{
	for (auto fingerboardStateIt = node->FingerboardStates.CreateIterator(); fingerboardStateIt; ++fingerboardStateIt)
	{
		for (auto fingerStateIt = fingerboardStateIt->FingerStates.CreateIterator(); fingerStateIt; ++fingerStateIt)
		{
			for (int iPin = 0; iPin < fingerStateIt->IsPinPressing.Num(); iPin++)
			{
				if (fingerStateIt->IsPinPressing[iPin])
				{
					if (fingerStateIt->PinPressEndTick[iPin] <= note->StartTick)
					{
						fingerStateIt->IsPinPressing[iPin] = false;
					}
				}
				else
				{
					if (fingerStateIt->PinPressEndTick[iPin] != 0 && fingerStateIt->PinPressEndTick[iPin] < note->StartTick - FINGER_PRESS_ANIM_DURATION)
					{
						fingerStateIt->PinPressEndTick[iPin] = 0;
					}
				}
			}
		}
	}
}

void UStringInstrumentFingeringGraph::CalculateTransitionScores(UGraphNode *parentNode)
{
	for (auto childNodeIt = parentNode->Children.CreateIterator(); childNodeIt; ++childNodeIt)
	{
		TArray<FFingerboardState> possibleTransitions;
		for (int32 iFingerboardState = 0; iFingerboardState < parentNode->FingerboardStates.Num(); iFingerboardState++)
		{
			if (IsTransitionPossible(*childNodeIt, parentNode->FingerboardStates[iFingerboardState]))
			{
				possibleTransitions.Add(CreateTransition(*childNodeIt, parentNode->FingerboardStates[iFingerboardState]));
			}
		}

		if (possibleTransitions.Num() != 0)
		{
			FFingerboardState bestTransition;
			int32 bestScore = MAXINT32;

			for (int32 iTransition = 0; iTransition < possibleTransitions.Num(); iTransition++)
			{
				if (possibleTransitions[iTransition].ParentScore < bestScore)
				{
					bestScore = possibleTransitions[iTransition].ParentScore;
					bestTransition = possibleTransitions[iTransition];
				}
			}

			bestTransition.ParentIndex = (*childNodeIt)->Parents.Num();
			(*childNodeIt)->Parents.Add(parentNode);
			(*childNodeIt)->FingerboardStates.Add(bestTransition);
		}	
	}
}

bool UStringInstrumentFingeringGraph::IsTransitionPossible(const UGraphNode *node, const FFingerboardState &fingerboardState) const
{
	if (node->FingerIndex == -1)
	{
		for (int iFingerState = 0; iFingerState < fingerboardState.FingerStates.Num(); iFingerState++)
		{
			FFingerState otherFingerState = fingerboardState.FingerStates[iFingerState];

			// If another finger is already pressing this string on a fret different than the required fret
			if (otherFingerState.IsPinPressing[node->StringPosition.String])
			{
				return false;
			}
		}

		return true;
	}

	FFingerState fingerState = fingerboardState.FingerStates[node->FingerIndex];

	// If the finger is already on the correct fret, return true
	if (fingerState.Fret == node->StringPosition.Fret)
	{
		return true;
	}

	// If the finger is not on the correct fret and is currently pressing, return false
	if (fingerState.Fret != node->StringPosition.Fret && (fingerState.IsPressing() || fingerState.IsTransitioning()))
	{
		return false;
	}

	// Look at other fingers
	for (int iFingerState = 0; iFingerState < fingerboardState.FingerStates.Num(); iFingerState++)
	{
		FFingerState otherFingerState = fingerboardState.FingerStates[iFingerState];

		// If another finger is already pressing this string on a fret different than the required fret
		if (otherFingerState.IsPinPressing[node->StringPosition.String])
		{
			return false;
		}

		// If a lower index finger is currently pressing between this finger and the target fret
		if (iFingerState < node->FingerIndex && otherFingerState.Fret >= node->StringPosition.Fret && otherFingerState.IsPressing())
		{
			return false;
		}

		// If a higher index finger is currently pressing between this finger and the target fret
		if (iFingerState > node->FingerIndex && otherFingerState.Fret <= node->StringPosition.Fret && otherFingerState.IsPressing())
		{
			return false;
		}
	}

	// No restrictions are violated
	return true;
}

FFingerboardState UStringInstrumentFingeringGraph::CreateTransition(const UGraphNode *node, const FFingerboardState &fingerboardState)
{
	FFingerboardState transitionState;

	for (int iFinger = 0; iFinger < mFingerCount; iFinger++)
	{
		FFingerState fingerState;
		fingerState.Fret = fingerboardState.FingerStates[iFinger].Fret;
		for (int iString = 0; iString < mStringCount; iString++)
		{
			fingerState.IsPinPressing.Add(fingerboardState.FingerStates[iFinger].IsPinPressing[iString]);
			fingerState.PinPressEndTick.Add(fingerboardState.FingerStates[iFinger].PinPressEndTick[iString]);
		}

		transitionState.FingerStates.Add(fingerState);
	}

	if (node->FingerIndex != -1)
	{
		transitionState.FingerStates[node->FingerIndex].Fret = node->StringPosition.Fret;
		transitionState.FingerStates[node->FingerIndex].IsPinPressing[node->StringPosition.String] = true;
		transitionState.FingerStates[node->FingerIndex].PinPressEndTick[node->StringPosition.String] = node->Note->GetEndTick();
	}
	
	transitionState.ParentScore = GetTransitionScore(node, fingerboardState, transitionState);

	return transitionState;
}

int32 UStringInstrumentFingeringGraph::GetTransitionScore(const UGraphNode *node, const FFingerboardState &oldState, const FFingerboardState &newState)
{
	int32 transitionScore = 0;

	transitionScore += FMath::Max<int8>(0, node->FingerIndex);
	transitionScore += node->StringPosition.Fret;

	if (node->FingerIndex != -1 && oldState.FingerStates[node->FingerIndex].Fret != 0)
	{
		transitionScore += FMath::Abs(oldState.FingerStates[node->FingerIndex].Fret - newState.FingerStates[node->FingerIndex].Fret);
	}

	return oldState.ParentScore + transitionScore;
}

void UStringInstrumentFingeringGraph::BuildOptimalFingering(UGraphNode *node)
{
	if (node->Note == nullptr) return;

	UGraphNode *bestParent = nullptr;
	int32 bestScore = MAXINT32;

	for (auto fingerboardStateIt = node->FingerboardStates.CreateIterator(); fingerboardStateIt; ++fingerboardStateIt)
	{
		if (fingerboardStateIt->ParentScore < bestScore)
		{
			bestScore = fingerboardStateIt->ParentScore;
			bestParent = node->Parents[fingerboardStateIt->ParentIndex];
		}
	}

	BuildOptimalFingering(bestParent);

	node->Parents.Empty();
	node->Children.Empty();
	node->FingerboardStates.Empty();

	OptimalFingering.Add(node);
}

