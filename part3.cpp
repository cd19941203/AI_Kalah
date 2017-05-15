#include <iostream>
#include <climits>
using namespace std;

const int houseLength = 14;

//(1) You need to call listSuccessors() to expand the minimax tree
//(2) If there is no successor(which means it is a game-over node), call finalScoring() and evaluate() to get mValue
//(3) If the expanded node is at the depth of depthMAX, call evaluate() to get mValue
bool noFurtherAction(int house[]); //1
bool relocation(int nowHouse[], int pickedHouse, int nextHouse[]);//2
int listSuccessors(int house[], int successor[], bool isMaxPlayer); //4-1
int evaluate(int house[]); //4-2
void finalScoring(int house[]); //5

int minimaxDecision(int house[], int depthMAX);
void maxValue(int house[], int depth, int depthMAX, int & Value_max, int & maxAction);
void minValue(int house[], int depth, int depthMAX, int & Value_min, int & minAction);

int main()
{
	int cases;
	cin >> cases;
	while (cases--)
	{
		int house[14] = { 0 };
		int depthMAX;

		for (int i = 0; i < 14; i++)
			cin >> house[i];
		cin >> depthMAX;

		cout << minimaxDecision(house, depthMAX) << endl;
	}
	return 0;
}


int minimaxDecision(int house[], int depthMAX)
{
	int Value_max, maxAction;
	maxValue(house, 1, depthMAX, Value_max, maxAction);
	return maxAction;
}

void maxValue(int house[], int depth, int depthMAX, int & Value_max, int & maxAction)
{
	Value_max = INT_MIN;
	if (noFurtherAction(house))
	{
		finalScoring(house);
		Value_max = evaluate(house);
	}
	else if (depth == depthMAX)
	{
		Value_max = evaluate(house);
	}
	else
	{
		int successor[14] = { 0 };
		int nextHouse[14] = { 0 };
		int len = listSuccessors(house, successor, true);

		for (int i = 0; i < len; i++)
		{
			int value, action;
			/*relocation(house, successor[i], nextHouse);
			minValue(nextHouse, depth + 1, depthMAX, value, action);*/
			
			if (relocation(house, successor[i], nextHouse))
				maxValue(nextHouse, depth + 1, depthMAX, value, action);
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
	Value_min = INT_MAX;
	if (noFurtherAction(house))
	{
		finalScoring(house);
		Value_min = evaluate(house);
	}
	else if (depth == depthMAX)
	{
		Value_min = evaluate(house);
	}
	else
	{
		int successor[14] = { 0 };
		int nextHouse[14] = { 0 };
		int len = listSuccessors(house, successor, false);

		for (int i = 0; i < len; i++)
		{
			int value, action;
			/*relocation(house, successor[i], nextHouse);
			maxValue(nextHouse, depth + 1, depthMAX, value, action);*/
			
			if (relocation(house, successor[i], nextHouse))
				minValue(nextHouse, depth + 1, depthMAX, value, action);
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
	for (int i = 0; i < 6; i++)
	{
		flag = flag & !house[i];
		flag2 = flag2 & !house[i + 7];
	}
	if (flag || flag2)
		return true;
	return false;
}

int listSuccessors(int house[], int successor[], bool isMaxPlayer)
{
	int size = 0;
	if (isMaxPlayer)
	{
		for (int i = 7; i < 13; i++)
			if (house[i] != 0)
				successor[size++] = i;
		return size;
	}
	else
	{
		for (int i = 0; i < 6; i++)
			if (house[i] != 0)
				successor[size++] = i;
		return size;
	}
}

int evaluate(int house[])
{
	return house[13] - house[6];
}

void finalScoring(int house[])
{
	for (int i = 0; i < 6; i++)
	{
		house[6] += house[i];
		house[13] += house[i + 7];
		house[i] = house[i + 7] = 0;
	}
}

bool relocation(int nowHouse[], int pickedHouse, int nextHouse[])
{
	int houseIndex[] = { houseLength / 2 - 1, houseLength - 1 };
	int myHouse = pickedHouse / 7;
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
		if (goHouse / 7 == myHouse)
			if (nextHouse[goHouse] == 1 && nextHouse[oppsite] > 0) {
				nextHouse[houseIndex[myHouse]] += nextHouse[goHouse] + nextHouse[oppsite];
				nextHouse[goHouse] = nextHouse[oppsite] = 0;
			}

	if (goHouse == houseIndex[myHouse])
		return true;
	return false;
}