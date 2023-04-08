#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <string>

using namespace std;

class TestClass
{
	int Value;
	public:bool TrueOrFalse = false;

	void SetTrueOrFalse(bool value);
	void PassByRef(std::string* message);
};

class Card
{
	public:string Suit;
	public:string Name;
	public:int Value;

	public:
	Card() {}

	public:
	Card(string suit, string name, int value)
	{
		Suit = suit;
		Name = name;
		Value = value;
	}
};

class Deck
{
	public:stack<Card> Cards = stack<Card>();
	public:string Trump;
	int LowestNumber;	//In a standard 28-card Euchre deck, this will always be 8.

	public: Deck() { }

	public:
	Deck(int lowest)
	{
		LowestNumber = lowest;
		this->Initialize();
		this->Shuffle();
	}
	
	void Initialize();
	public:void Shuffle();
	public:void AddCard(Card card);
	public:string GetOppositeSuit(string suit);
};

class EuchreTeam
{
	public:int Points = 0;
	public:string Name;
	public:bool AcceptedTrumpThisRound = false;
	public:bool WonThisRound = false;

	public:EuchreTeam() { }
	public:EuchreTeam(string name)
	{
		Name = name;
	}

	public:void AddPoints(int amt);
	public:void RemovePoint(int amt);
	public:void DisplayScore();
};

class EuchreBot
{
	public:vector<Card> Cards = vector<Card>();
	public:string Name;
	public:EuchreTeam Team;

	public:EuchreBot() {}
	public:
	EuchreBot(string name, EuchreTeam team)
	{
		Team = team;
		Name = name + " (" + Team.Name + ")";
	}

	public:void DrawCard(Deck &deck);
	public:bool CallTrump(Deck &deck);
};

class PlayedCard
{
	public:EuchreBot Player;
	public:Card CardPlayed;

	public:PlayedCard(EuchreBot player, Card card)
	{
		Player = player;
		CardPlayed = card;
	}
};

class Game
{
	Deck deck;
	vector<EuchreBot> Players = vector<EuchreBot>();
	EuchreTeam RedTeam = EuchreTeam("Red Team");
	EuchreTeam BlueTeam = EuchreTeam("Blue Team");
	int TurnCounter = 1;
	int TurnStarter = 0;
	bool GameOver = false;
	public:int WinningScore = 0;

	public:
	Game(int lowest, int winningScore)
	{
		deck = Deck(lowest);
		WinningScore = winningScore;
		Players.push_back(EuchreBot("Jimmy", RedTeam));
		Players.push_back(EuchreBot("Chuck", BlueTeam));
		Players.push_back(EuchreBot("George", RedTeam));
		Players.push_back(EuchreBot("Jeff", BlueTeam));

		StartTurn();
	}

	void StartTurn();
	void DealCards();
	void ReturnAllCards();
	void GivePoints(EuchreTeam &team, int amt);
	void EndGame(EuchreTeam winningTeam);
	void DetermineWin(vector<PlayedCard> CardsPlayed, string LeadingSuit);
};