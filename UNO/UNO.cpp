/*Isaac Reich
UNO*/

#include<iostream>
#include<conio.h>
#include<string>
#include<vector>
#include<ctime>

using namespace std;

struct Card {
	string color;
	int num;
	string special;
};

void createDeck(vector<Card> &deck);

void showCard(const Card &c, int i);

void showDeck(const vector<Card> &deck);

void Shuffle(vector<Card> &deck); //Shuffles the cards into random positions in the deck

void DealHand(vector<Card>&deck, vector<Card>&player_hand); //deals 7 cards from the draw pile into the hand

void NewGame(vector<Card>&deck, vector<Card>&player_hand, vector<Card>&cpu_hand, vector<Card>&discard_pile); //Generates a new game, creates the deck, shuffles it, deals the player’s and computer’s starting hand, and turns over the initial card for the discard pile.

void UpdateHand(vector<Card>&player_hand, vector<Card>&discard_pile, vector<Card>&deck, int card);

void PlayerTurn(vector<Card>&player_hand, vector<Card>&cpu_hand, vector<Card>&discard_pile, vector<Card>&deck);

void FinishActions(vector<Card>&deck, vector<Card>&receivingHand, vector<Card>&discard_pile);

void ComputerTurn(vector<Card>&deck, vector<Card>&cpu_hand, vector<Card>&player_hand, vector<Card>&discard_pile);

void Transfer(vector<Card>&discard_pile, vector<Card>&deck);

bool FirstCard(vector<Card>&deck); //so first card isn't draw two, draw four, or skip 

int main()
{
	vector<Card>deck;
	vector<Card>player_hand(7);
	vector<Card>cpu_hand(7);
	vector<Card>discard_pile;

	char again = 'y';

	while (again == 'y' || again == 'Y')
	{
		system("cls");

		NewGame(deck, player_hand, cpu_hand, discard_pile);

		do
		{

			Transfer(discard_pile, deck);

			PlayerTurn(player_hand, cpu_hand, discard_pile, deck);

			if (player_hand.size() != 0)
				ComputerTurn(deck, cpu_hand, player_hand, discard_pile);

		} while (player_hand.size() != 0 || cpu_hand.size() != 0);


		if (player_hand.size() == 0)
			cout << "Congrats. Player wins :) " << endl;

		else if (cpu_hand.size() == 0)
			cout << "Sorry. Computer wins :( " << endl;


		cout << "Play again (y/n)? ";
		cin >> again;

	}

	_getch();
	return 0;

	
}

void createDeck(vector<Card> &deck)
{

	string c = "red";
	Card temp;
	temp.color = c;
	for (int i = 0; i<80; i++)
	{
		if (i == 20)
		{
			c = "green";

		}
		else if (i == 40)
		{

			c = "blue";
		}
		else if (i == 60)
		{
			c = "yellow";
		}

		if (i % 10 == 0)
		{
			temp.color = c;

		}
		if (i % 20 == 0)
		{
			i++;
			for (int j = 0; j<2; j++)
			{
				temp.num = 10;
				temp.special = "draw two";
				deck.push_back(temp);
				temp.special = "skip";
				deck.push_back(temp);
				temp.special = "";
			}
		}
		temp.num = i % 10;
		if (i == 0)
			temp.num = 1;
		deck.push_back(temp);

	}

	temp.num = 10;
	temp.color = "wild";
	temp.special = "";
	for (int i = 0; i<4; i++)
	{
		deck.push_back(temp);
	}
	temp.special = "draw four";
	for (int i = 0; i<4; i++)
	{
		deck.push_back(temp);
	}


}

void showCard(const Card &c, int i)
{
	cout << "[" << i << "][";
	if (c.num != 10)
		cout << c.num << " ";
	cout.width(6);
	cout << c.color << " ";
	cout << c.special << "]" << endl;
}

void showDeck(const vector<Card> &deck)
{
	cout << "Hand: " << endl;
	for (int i = 0; i<deck.size(); i++)
	{
		showCard(deck[i], i);
	}
	cout << endl;
}

void Shuffle(vector<Card> &deck)
{
	srand(time(0));

	for (int i = 0; i < deck.size(); i++)
	{
		int switchTo = rand() % ((deck.size() - 1) - 0 + 1) + 0;

		Card current = deck[switchTo];

		deck[switchTo] = deck[i];

		deck[i] = current;
	}
}

void DealHand(vector<Card>&deck, vector<Card>&v)
{

	for (int i = 0; i < v.size(); i++)
	{
		v[i] = deck[deck.size() - 1];

		deck.pop_back(); //After the last inex value on deck was assigned to player, this card can be eliminated from deck
	}

}

void NewGame(vector<Card>&deck, vector<Card>&player_hand, vector<Card>&cpu_hand, vector<Card>&discard_pile)
{
	deck.clear();
	system("cls");

	createDeck(deck);

	Shuffle(deck);

	DealHand(deck, player_hand);
	DealHand(deck, cpu_hand);

	do
	{
		Shuffle(deck);

	} while (FirstCard(deck)); //so top card won't be special


	
	discard_pile.push_back(deck.back());
	deck.pop_back();


	for (int i = 0; i < player_hand.size(); i++)
		showCard(player_hand[i], i);

	cout << endl;
	cout << "Top Card: " << endl;
	showCard(discard_pile[0], 0);
}

void UpdateHand(vector<Card>&hand, vector<Card>&discard_pile, vector<Card>&deck, int card)
{
	if (card == -1)
	{
		hand.push_back(deck.back());
		deck.pop_back();
	}
	else
	{
		discard_pile.push_back(hand[card]);
		hand[card] = hand.back();
		hand.pop_back();
	}

}

void PlayerTurn(vector<Card>&player_hand, vector<Card>&cpu_hand, vector<Card>&discard_pile, vector<Card>&deck)
{
	int card;
	int over_index;
	int invalid;

	do
	{
		invalid = 0; //to check the various ways that the card asked to be placed if it is valid
		over_index = 0;

		cout << "Choose card to play down (-1 to draw instead) : ";
		cin >> card;

		if (card == -1)
		{
			UpdateHand(player_hand, discard_pile, deck, card);
		}
		else if (card >= player_hand.size())
		{
			cout << "Invalid index - choose again" << endl;
			over_index++;
		}
		else
		{
			//as long as one is true, the card qill be valid but invalid will hit 3 if all are not true
			if (player_hand[card].color != discard_pile.back().color)
				invalid++;
			if (player_hand[card].num != discard_pile.back().num)
				invalid++;
			if (player_hand[card].special != "wild" && player_hand[card].special != "wild draw four")
				invalid++;


			if (invalid == 3)
				cout << "Invalid match - choose again" << endl;
			else
				UpdateHand(player_hand, discard_pile, deck, card);

		}

	} while (invalid == 3 || over_index == 1 || card == -1);

	cout << "Player: " << endl;
	for (int i = 0; i < player_hand.size(); i++)
		showCard(player_hand[i], i);

	if (player_hand[card].special == "draw four" || player_hand[card].special == "draw two")
		FinishActions(deck, cpu_hand, discard_pile);

	//Repeats this function if skip is played. Put this here instead of FinishActions because of conveniency
	if (player_hand[card].special == "skip")
		PlayerTurn(player_hand, cpu_hand, discard_pile, deck);

}

void FinishActions(vector<Card>&deck, vector<Card>&receivingHand, vector<Card>&discard_pile)
{

	if (discard_pile.back().special == "draw four")
	{
		for (int i = deck.size() - 1; i <= deck.size() - 5; i--)
			receivingHand.resize(1, deck[i]);
	}
	else if (discard_pile.back().special == "draw two")
	{
		for (int i = deck.size() - 1; i <= deck.size() - 3; i--)
			receivingHand.resize(1, deck[i]);
	}

}

void ComputerTurn(vector<Card>&deck, vector<Card>&cpu_hand, vector<Card>&player_hand, vector<Card>&discard_pile)
{
	cout << "Computer: " << endl;
	for (int i = 0; i < cpu_hand.size(); i++)
		showCard(cpu_hand[i], i);

	for (int i = 0; i < cpu_hand.size(); i++)
	{
		if (cpu_hand[i].special == "draw four")
		{
			UpdateHand(cpu_hand, discard_pile, deck, i);

			FinishActions(deck, player_hand, discard_pile);
		}

	}

	for (int i = 0; i < cpu_hand.size(); i++)
	{
		if (cpu_hand[i].special == "draw two")
		{
			UpdateHand(cpu_hand, discard_pile, deck, i);

			FinishActions(deck, player_hand, discard_pile);
		}

	}

	for (int i = 0; i < cpu_hand.size(); i++)
	{
		//removes the skip card from cpu deck and puts on discard pile. Calls com turn again
		if (cpu_hand[i].special == "skip")
		{
			UpdateHand(cpu_hand, discard_pile, deck, i);

			ComputerTurn(deck, cpu_hand, player_hand, discard_pile);
		}
		//Here instead of FinishActions because easier
	}

	//So that if it leaves loop with value 1, it won't hit the draw card if statement. If it leaves with -1 then there was no card to put down and it will draw card
	int hadCard = 0;

	//iterations for while loop
	int i = 0;

	while (hadCard == 0)
	{

		if (cpu_hand[i].color == discard_pile.back().color)
		{
			UpdateHand(cpu_hand, discard_pile, deck, i);
			hadCard++;
		}
		else if (cpu_hand[i].num != discard_pile.back().num)
		{
			UpdateHand(cpu_hand, discard_pile, deck, i);
			hadCard++;
		}
		else;

		i++;

		if (i == cpu_hand.size())
			hadCard = -1;

	}

	if (hadCard == -1)
	{
		UpdateHand(cpu_hand, discard_pile, deck, hadCard);
	}

	cout << "Top Card: ";
	showCard(discard_pile.back(), discard_pile.size() - 1);

}

void Transfer(vector<Card>&discard_pile, vector<Card>&deck)
{

	if (deck.size() == 0)
	{
		Card saveTopCard = discard_pile.back();

		deck = discard_pile;
		deck.pop_back();

		Shuffle(deck);

		discard_pile.resize(1, saveTopCard);
	}

}

bool FirstCard(vector<Card>&deck) //so first card isn't draw two, draw four, or skip 
{

	if (deck.back().special == "draw four" || deck.back().special == "draw two" || deck.back().special == "skip")
	{
		return true;
	}
	else
	{
		return false;
	}
}