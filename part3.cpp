#include <iostream>
#include <climits>
using namespace std;

#define houseLength 14

bool noFurtherAction(int house[]);									// part 1
bool relocation(int nowHouse[], int pickedHouse, int nextHouse[]);	// part 2
int listSuccessors(int house[], int successor[], bool isMaxPlayer);	// part 4-1
int evaluate(int house[]);											// part 4-2
void finalScoring(int house[]);										// part 5

//(1) You need to call listSuccessors() to expand the minimax tree
//(2) If there is no successor(which means it is a game-over node), call finalScoring() and evaluate() to get mValue
//(3) If the expanded node is at the depth of depthMAX, call evaluate() to get mValue
int minimaxDecision(int house[], int depthMAX);
void maxValue(int house[], int depth, int depthMAX, int & Value_max, int & maxAction);
void minValue(int house[], int depth, int depthMAX, int & Value_min, int & minAction);

int main()
{
	int cases;
	cin >> cases;
	while (cases--)
	{
		int house[houseLength] = { 0 };
		int depthMAX;

		for (int i = 0; i < houseLength; i++)
			cin >> house[i];
		cin >> depthMAX;

		cout << minimaxDecision(house, depthMAX) << endl;
	}
	return 0;
}

int minimaxDecision(int house[], int depthMAX)
{
	int Value_max, maxAction;
	maxValue(house, 0, depthMAX, Value_max, maxAction);
	return maxAction;
}

void maxValue(int house[], int depth, int depthMAX, int & Value_max, int & maxAction)
{
	int successor[houseLength] = { 0 };
	int nextHouse[houseLength] = { 0 };
	int len = listSuccessors(house, successor, true);
	Value_max = INT_MIN;

	if (noFurtherAction(house))
	{
		finalScoring(house);
		Value_max = evaluate(house);
		maxAction = -1;
	}
	else if (depth >= depthMAX)
	{
		Value_max = evaluate(house);
		maxAction = -1;
	}
	else
	{
		for (int i = 0; i < len; i++)
		{
			int value, action;

			if (relocation(house, successor[i], nextHouse))
				maxValue(nextHouse, depth + 2, depthMAX, value, action);
			else
				minValue(nextHouse, depth + 1, depthMAX, value, action);

			if (Value_max < value)
			{
				Value_max = value;
				maxAction = successor[i];
			}
		}
	}
}

void minValue(int house[], int depth, int depthMAX, int & Value_min, int & minAction)
{
	int successor[houseLength] = { 0 };
	int nextHouse[houseLength] = { 0 };
	int len = listSuccessors(house, successor, false);
	Value_min = INT_MAX;

	if (noFurtherAction(house))
	{
		finalScoring(house);
		Value_min = evaluate(house);
		minAction = -1;
	}
	else if (depth >= depthMAX)
	{
		Value_min = evaluate(house);
		minAction = -1;
	}
	else
	{
		for (int i = 0; i < len; i++)
		{
			int value, action;

			if (relocation(house, successor[i], nextHouse))
				minValue(nextHouse, depth + 2, depthMAX, value, action);
			else
				maxValue(nextHouse, depth + 1, depthMAX, value, action);

			if (Value_min > value)
			{
				Value_min = value;
				minAction = successor[i];
			}
		}
	}
}

bool noFurtherAction(int house[])
{
	bool flag = true, flag2 = true;
	for (int i = 0, max = houseLength / 2 - 1; i < max; i++)
	{
		flag = flag & !house[i];
		flag2 = flag2 & !house[i + houseLength / 2];
	}
	if (flag || flag2)
		return true;
	return false;
}

int listSuccessors(int house[], int successor[], bool isMaxPlayer)
{
	int size = 0;
	int offset = isMaxPlayer ? houseLength / 2 : 0;

	for (int i = 0 + offset, max = houseLength / 2 - 1 + offset; i < max; i++)
		if (house[i] != 0)
			successor[size++] = i;
	return size;
}

int evaluate(int house[])
{
	return house[houseLength - 1] - house[houseLength / 2 - 1];
}

void finalScoring(int house[])
{
	for (int i = 0; i < 6; i++)
	{
		house[houseLength / 2 - 1] += house[i];
		house[houseLength - 1] += house[i + houseLength / 2];
		house[i] = house[i + houseLength / 2] = 0;
	}
}

bool relocation(int nowHouse[], int pickedHouse, int nextHouse[])
{
	int houseIndex[] = { houseLength / 2 - 1, houseLength - 1 };
	int myHouse = pickedHouse / (houseLength / 2);
	int left = nowHouse[pickedHouse];
	int goHouse = pickedHouse + 1;

	for (int i = 0; i < houseLength; i++) {
		if (i == pickedHouse) {
			nextHouse[i] = 0;
			continue;
		}
		nextHouse[i] = nowHouse[i];
	}

	while (left) {
		goHouse = goHouse % houseLength;
		if (goHouse != houseIndex[!myHouse]) {
			nextHouse[goHouse]++;
			left--;
		}
		goHouse++;
	} goHouse--;

	int oppsite = houseLength - 2 - goHouse;
	if (oppsite != houseIndex[0] && oppsite != houseIndex[1])
		if (goHouse / (houseLength / 2) == myHouse)
			if (nextHouse[goHouse] == 1 && nextHouse[oppsite] > 0) {
				nextHouse[houseIndex[myHouse]] += nextHouse[goHouse] + nextHouse[oppsite];
				nextHouse[goHouse] = nextHouse[oppsite] = 0;
			}

	if (goHouse == houseIndex[myHouse])
		return true;
	return false;
}
