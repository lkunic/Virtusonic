// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringInstrumentFingeringGraph.h"

void UStringInstrumentFingeringGraph::Init(const int8 fingerCount, const int8 stringCount, const int8 fretCount)
{
	mFingerCount = fingerCount;
	mStringCount = stringCount;
	mFretCount = fretCount;

	// Initialize the empty root node for the graph
	mRoot = NewObject<UGraphNode>();

	FStringPosition position;
	position.Fret = -1;
	position.String = -1;
	mRoot->StringPosition = position;
	mRoot->FingerIndex = -1;
	mRoot->Note = nullptr;
	mRoot->Children.Empty();

	// Fingerboard state is set so that all fingers are on fret MAXINT (meaning in the resting pose)
	FFingerboardState fingerboardState;
	fingerboardState.ParentIndex = -1;
	fingerboardState.ParentFingerboardStateIndex = 0;
	fingerboardState.ParentScore = 0;
	for (int iFinger = 0; iFinger < mFingerCount; iFinger++)
	{
		FFingerState fingerState;
		fingerState.Fret = MAX_int8;
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
	// Each note is represented by a layer containing all possible finger/position combinations (regardless of the current fingerboard state),
	// with all nodes fully connected to the nodes of the previous layer
	TArray<UGraphNode*> currentLayer;

	for (int8 iPosition = 0; iPosition < notePositions.Num(); iPosition++)
	{
		if (notePositions[iPosition].Fret == 0)
		{
			// The note is on an empty string (fret 0), so no finger needs to press it
			UGraphNode *node = NewObject<UGraphNode>();
			node->StringPosition = notePositions[iPosition];
			node->Note = note;
			node->FingerboardStates.Empty();
			node->Parents.Empty();
			node->Children.Empty();
			node->FingerIndex = -1;

			for (UGraphNode *parentNode : mLastLayer)
			{
				// Connect all previous layer nodes to this node
				parentNode->Children.Add(node);
			}

			currentLayer.Add(node);
		}
		else
		{
			for (int8 iFinger = 0; iFinger < mFingerCount; iFinger++)
			{
				// Initialize a node for the finger/position combination 
				// (fingerboard states, parents and children will be updated after the graph is fully built)
				UGraphNode *node = NewObject<UGraphNode>();
				node->StringPosition = notePositions[iPosition];
				node->Note = note;
				node->FingerboardStates.Empty();
				node->Parents.Empty();
				node->Children.Empty();
				node->FingerIndex = iFinger;


				for (UGraphNode *parentNode : mLastLayer)
				{
					// Connect all previous layer nodes to this node
					parentNode->Children.Add(node);
				}

				currentLayer.Add(node);
			}
		}
	}

	mLastLayer = currentLayer;
}

void UStringInstrumentFingeringGraph::CalculateFingering()
{
	// Reset the last layer and add the root node
	mLastLayer.Empty();
	mLastLayer.Add(mRoot);

	// The first layer that needs to be processed is the root's children
	TArray<UGraphNode*> currentLayer = mRoot->Children;

	// Iterate through all layers (all notes)
	while (currentLayer.Num() != 0)
	{
		for (UGraphNode *parentNode : mLastLayer)
		{
			// Check what changed in the fingerboard state of the parent node at the start tick of the new note
			UpdateFingerboardStates(parentNode, currentLayer[0]->Note->StartTick);

			// Calculate the transition scores from the parent node to all its children nodes (all viable nodes in the current layer)
			CalculateTransitionScores(parentNode);
		}

		mLastLayer = currentLayer;

		// The graph layers are fully connected, so currentLayer[0]->Children contains all of the next layer nodes
		currentLayer = currentLayer[0]->Children;
	}

	// Find the node with the lowest score in the last graph
	UGraphNode *bestNode = nullptr;
	FFingerboardState bestFingerboardState;
	int32 bestScore = MAX_int32;

	for (UGraphNode *endNode : mLastLayer)
	{
		for (auto fingerboardStateIt = endNode->FingerboardStates.CreateIterator(); fingerboardStateIt; ++fingerboardStateIt)
		{
			if (fingerboardStateIt->ParentScore < bestScore)
			{
				bestNode = endNode;
				bestFingerboardState = *fingerboardStateIt;
				bestScore = fingerboardStateIt->ParentScore;
			}
		}
	}

	// Backtrack through the graph from the best node in order to build the optimal fingering sequence
	BuildOptimalFingering(bestNode, bestFingerboardState);

	for (int iFinger = 0; iFinger < mFingerCount; iFinger++)
	{
		CleanupFingerMovement(0, iFinger, -1);
	}

	// Enable garbage collection on the private members
	mRoot = nullptr;
	mLastLayer.Empty();
}

void UStringInstrumentFingeringGraph::UpdateFingerboardStates(UGraphNode *node, const int32 tick)
{
	for (auto fingerboardStateIt = node->FingerboardStates.CreateIterator(); fingerboardStateIt; ++fingerboardStateIt)
	{
		for (auto fingerStateIt = fingerboardStateIt->FingerStates.CreateIterator(); fingerStateIt; ++fingerStateIt)
		{
			// If a finger is pressing and the press end tick is less than the current tick, the finger should release,
			// but it should be unavailable until FINGER_PASS_ANIM_DURATION has passed, so that it cannot be instantly
			// used again for playing another note on a different fret
			for (int iPin = 0; iPin < fingerStateIt->IsPinPressing.Num(); iPin++)
			{
				if (fingerStateIt->IsPinPressing[iPin])
				{
					if (fingerStateIt->PinPressEndTick[iPin] <= tick)
					{
						fingerStateIt->IsPinPressing[iPin] = false;
					}
				}
				else
				{
					// If the finger is no longer pressing but was pressing recently, 
					// check if enough time has passed and re-enable it by reseting the end tick time
					if (fingerStateIt->PinPressEndTick[iPin] != 0 && fingerStateIt->PinPressEndTick[iPin] < tick - FINGER_PRESS_ANIM_DURATION)
					{
						//fingerStateIt->Fret = MAXINT8;
						fingerStateIt->PinPressEndTick[iPin] = 0;
					}
				}
			}
		}
	}
}

void UStringInstrumentFingeringGraph::CalculateTransitionScores(UGraphNode *parentNode)
{
	// Calculate the transition scores for each child node if the transition is possible for
	// any of the current fingerboard states
	for (auto childNodeIt = parentNode->Children.CreateIterator(); childNodeIt; ++childNodeIt)
	{
		TArray<FFingerboardState> possibleNewStates;
		for (int32 iFingerboardState = 0; iFingerboardState < parentNode->FingerboardStates.Num(); iFingerboardState++)
		{
			FFingerboardState newState;
			if (CreateTransitionFingerboardState(*childNodeIt, parentNode->FingerboardStates[iFingerboardState], newState))
			{
				newState.ParentFingerboardStateIndex = iFingerboardState;
				possibleNewStates.Add(newState);
			}
		}

		if (possibleNewStates.Num() != 0)
		{
			// Iterate through the possible transitions to find the best one and set is as the child's predecessor
			FFingerboardState bestTransition;
			int32 bestScore = MAX_int32;

			for (int32 iTransition = 0; iTransition < possibleNewStates.Num(); iTransition++)
			{
				if (possibleNewStates[iTransition].ParentScore < bestScore)
				{
					bestScore = possibleNewStates[iTransition].ParentScore;
					bestTransition = possibleNewStates[iTransition];
				}
			}

			bestTransition.ParentIndex = (*childNodeIt)->Parents.Num();
			(*childNodeIt)->Parents.Add(parentNode);
			(*childNodeIt)->FingerboardStates.Add(bestTransition);
		}	
	}
}

bool UStringInstrumentFingeringGraph::CreateTransitionFingerboardState(const UGraphNode *node, const FFingerboardState &fingerboardState, FFingerboardState &outState)
{
	// Copy the given parent fingerboard state
	for (int iFinger = 0; iFinger < mFingerCount; iFinger++)
	{
		FFingerState fingerState;
		fingerState.Fret = fingerboardState.FingerStates[iFinger].Fret;
		for (int iString = 0; iString < mStringCount; iString++)
		{
			fingerState.IsPinPressing.Add(fingerboardState.FingerStates[iFinger].IsPinPressing[iString]);
			fingerState.PinPressEndTick.Add(fingerboardState.FingerStates[iFinger].PinPressEndTick[iString]);
		}

		outState.FingerStates.Add(fingerState);
	}

	if (node->FingerIndex == -1)
	{
		// The node represents playing an empty string (no finger is pressing it, fret 0), 
		// so it can be played if no finger is currently pressing that string on any fret
		for (int iFingerState = 0; iFingerState < fingerboardState.FingerStates.Num(); iFingerState++)
		{
			FFingerState fingerState = fingerboardState.FingerStates[iFingerState];

			// If the finger is already pressing this string on a fret different than the required fret
			if (fingerState.IsPinPressing[node->StringPosition.String])
			{
				return false;
			}
		}
	}
	else
	{
		// The node represents using a finger to press a string at a specified position
		FFingerState &fingerState = outState.FingerStates[node->FingerIndex];

		// If the finger is not on the correct fret and is currently pressing or was recently pressing (is in transition phase), return false
		if (fingerState.Fret != node->StringPosition.Fret && (fingerState.IsPressing() || fingerState.IsTransitioning()))
		{
			return false;
		}

		fingerState.Fret = node->StringPosition.Fret;
		fingerState.IsPinPressing[node->StringPosition.String] = true;
		fingerState.PinPressEndTick[node->StringPosition.String] = node->Note->GetEndTick();

		for (int iFinger = node->FingerIndex - 1; iFinger >= 0; iFinger--)
		{
			int8 highestFret = GetHighestFretForFinger(iFinger, outState);
			if (highestFret == -1)
			{
				return false;
			}

			outState.FingerStates[iFinger].Fret = highestFret;
		}
		for (int iFinger = node->FingerIndex + 1; iFinger < outState.FingerStates.Num(); iFinger++)
		{
			int8 lowestFret = GetLowestFretForFinger(iFinger, outState);
			if (lowestFret == -1)
			{
				return false;
			}

			outState.FingerStates[iFinger].Fret = lowestFret;
		}	

	}

	outState.ParentScore = GetTransitionScore(node, fingerboardState, outState);

	return true;
}

int8 UStringInstrumentFingeringGraph::GetHighestFretForFinger(const int8 fingerIndex, const FFingerboardState &fingerboardState)
{
	if (fingerIndex == fingerboardState.FingerStates.Num() - 1)
	{
		// This should never be called on the last finger in the sequence
		return -1;
	}

	FFingerState fingerState = fingerboardState.FingerStates[fingerIndex];
	FFingerState nextFingerState = fingerboardState.FingerStates[fingerIndex + 1];

	if (fingerState.IsPressing() || fingerState.IsTransitioning())
	{
		// If the finger is pressing and higher than the next finger, the fingerboard is in an invalid state
		if (nextFingerState.Fret <= fingerState.Fret)
		{
			return -1;
		}

		// If the finger is pressing and lower than the next finger, let it stay at the same position
		return fingerState.Fret;
	}
	else
	{
		// If the finger is not pressing, move it so that it's one fret behind the next finger
		// Make sure the finger is never moved below the minimum position (at which the first finger is on the first fret,
		// and each of the following fingers can't get below their respective position relative to the first finger)
		int8 result = nextFingerState.Fret - 1;
		return result > fingerIndex ? result : -1;
	}
}

int8 UStringInstrumentFingeringGraph::GetLowestFretForFinger(const int8 fingerIndex, const FFingerboardState &fingerboardState)
{
	if (fingerIndex == 0)
	{
		// This should never be called on the first finger in the sequence
		return -1;
	}

	FFingerState fingerState = fingerboardState.FingerStates[fingerIndex];
	FFingerState previousFingerState = fingerboardState.FingerStates[fingerIndex - 1];

	if (fingerState.IsPressing() || fingerState.IsTransitioning())
	{
		// If the finger is pressing and higher than the next finger, the fingerboard is in an invalid state
		if (previousFingerState.Fret >= fingerState.Fret)
		{
			return -1;
		}

		// If the finger is pressing and lower than the next finger, let it stay at the same position
		return fingerState.Fret;
	}
	else
	{
		// If the finger is not pressing, move it so that it's one fret after the next finger
		// Make sure the finger is never moved above the maximum position (at which the last finger is on the last fret,
		// and each of the previous fingers can't get above their respective position relative to the last finger)
		int8 result = previousFingerState.Fret + 1;
		return result <= (mFretCount - mFingerCount + fingerIndex + 1) ? result : -1;
	}
}


int32 UStringInstrumentFingeringGraph::GetTransitionScore(const UGraphNode *node, const FFingerboardState &oldState, const FFingerboardState &newState)
{
	int32 transitionScore = 0;

	// The higher the finger index, the higher the score
	transitionScore += FMath::Max<int8>(0, node->FingerIndex);

	// Favor fret positions around 6 (should look best visually)
	transitionScore += FRET_POSITION_FACTOR * FMath::Abs(node->StringPosition.Fret - 6);

	// Include an additional cost for empty strings (they are boring)
	if (node->StringPosition.Fret == 0)
	{
		transitionScore += EMPTY_STRING_COST;
	}

	// Total distance traveled by all fingers and extra cost for pressing/lifting pins
	for (int8 iFinger = 0; iFinger < oldState.FingerStates.Num(); iFinger++)
	{
		if (oldState.FingerStates[iFinger].Fret == MAX_int8) continue;
		transitionScore += FRET_DISTANCE_FACTOR * FMath::Abs(oldState.FingerStates[iFinger].Fret - newState.FingerStates[iFinger].Fret);
	}

	// Finger span
	transitionScore += FINGER_SPAN_FACTOR * (newState.FingerStates.Last().Fret - newState.FingerStates[0].Fret);

	return oldState.ParentScore + transitionScore;
}

void UStringInstrumentFingeringGraph::BuildOptimalFingering(UGraphNode *node, const FFingerboardState &state)
{
	if (node->Note == nullptr) return;

	UGraphNode *parent = node->Parents[state.ParentIndex];
	FFingerboardState parentState = parent->FingerboardStates[state.ParentFingerboardStateIndex];

	BuildOptimalFingering(parent, parentState);

	node->Parents.Empty();
	node->Children.Empty();
	node->FingerboardStates.Empty();
	node->FingerboardStates.Add(state);

	OptimalFingering.Add(node);
}

bool UStringInstrumentFingeringGraph::CleanupFingerMovement(const int32 noteIndex, const int8 fingerIndex, const int8 lastPressedFret)
{
	// Check if we've reached the end of the recursion
	if (noteIndex >= OptimalFingering.Num())
	{
		return false;
	}

	// This value determines whether the finger plays two same notes in a row (in which case it should stay at the same 
	// fret without moving inbetween), and whether the movement graph should be 'flattened', which means that all movement
	// that might happen between those two values should be removed.
	bool shouldFlatten;
	if (OptimalFingering[noteIndex]->FingerIndex == fingerIndex)
	{
		// This finger presses a note, so update the currently pressed fret for the recursion and go down one step
		shouldFlatten = CleanupFingerMovement(noteIndex + 1, fingerIndex, OptimalFingering[noteIndex]->StringPosition.Fret);
	}
	else
	{
		// The finger doesn't press a note, so the last pressed fret is still the same, continue with the recursion
		shouldFlatten = CleanupFingerMovement(noteIndex + 1, fingerIndex, lastPressedFret);
	}

	if (OptimalFingering[noteIndex]->FingerIndex != fingerIndex)
	{
		// This note isn't played by this finger, use the received value to flatten the finger movement if required
		if (shouldFlatten)
		{
			if (GetHighestFretForFinger(fingerIndex, OptimalFingering[noteIndex]->FingerboardStates[0]) >= lastPressedFret &&
				GetLowestFretForFinger(fingerIndex, OptimalFingering[noteIndex]->FingerboardStates[0]) <= lastPressedFret)
			{
				OptimalFingering[noteIndex]->FingerboardStates[0].FingerStates[fingerIndex].Fret = lastPressedFret;
			}

			//OptimalFingering[noteIndex]->FingerboardStates[0].FingerStates[fingerIndex].Fret = lastPressedFret;
		}

		return shouldFlatten;
	}
	else
	{
		// The note is played by this finger. If the fret to be pressed is the same as the previous fret,
		// return true so that any movement between playing those two notes can be removed
		if (OptimalFingering[noteIndex]->StringPosition.Fret == lastPressedFret)
		{
			return true;
		}

		if (lastPressedFret != -1 && noteIndex > 0 && 
			OptimalFingering[noteIndex - 1]->StringPosition.Fret != OptimalFingering[noteIndex]->StringPosition.Fret &&
			OptimalFingering[noteIndex - 1]->StringPosition.Fret != lastPressedFret)
		{
			return true;
		}

		return false;
	}
}
