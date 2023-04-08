#include "pch.h"

int Add(int x, int y) { return x + y; }

void TestClass::SetTrueOrFalse(bool value) { TrueOrFalse = value; }
void TestClass::PassByRef(std::string* message) { std::cout << *message << std::endl; }

Card GetLargestCard(vector<Card> Cards)
{
    if (Cards.empty())
        throw;

    Card biggest = Cards[0];
    for (int i = 1; i < Cards.size(); i++)
    {
        if (Cards[i].Value > biggest.Value)
        {
            biggest = Cards[i];
        }
    }

    return biggest;
}

vector<Card> GetMatchingSuits(vector<Card> Cards, string suit)
{
    vector<Card> MatchingSuits = vector<Card>();

    for (int i = 0; i < Cards.size(); i++)
    {
        if (Cards[i].Suit == suit)
        {
            MatchingSuits.push_back(Cards[i]);
        }
    }

    return MatchingSuits;
}

PlayedCard GetLargestCard(vector<PlayedCard> Cards)
{
    if (Cards.empty())
        throw;

    PlayedCard biggest = Cards[0];
    for (int i = 1; i < Cards.size(); i++)
    {
        if (Cards[i].CardPlayed.Value > biggest.CardPlayed.Value)
        {
            biggest = Cards[i];
        }
    }

    return biggest;
}

vector<PlayedCard> GetMatchingSuits(vector<PlayedCard> Cards, string suit)
{
    vector<PlayedCard> MatchingSuits = vector<PlayedCard>();

    for (int i = 0; i < Cards.size(); i++)
    {
        if (Cards[i].CardPlayed.Suit == suit)
        {
            MatchingSuits.push_back(Cards[i]);
        }
    }

    return MatchingSuits;
}

bool EuchreBot::CallTrump(Deck& deck)
{
    vector<Card> MatchingSuits = GetMatchingSuits(Cards, deck.Trump);

    bool AcceptTheTrump = !MatchingSuits.empty();    //The player has at least one card which is either the trump or its opposite.

    if (AcceptTheTrump)
    {
        cout << Name << " has accepted the trump." << endl;
    }

    Team.AcceptedTrumpThisRound = AcceptTheTrump;
    return AcceptTheTrump;
}

void EuchreTeam::AddPoints(int amt)
{
    Points += amt;
    cout << this->Name << " scored " << to_string(amt) << " point(s)." << endl;
    this->DisplayScore();
}

void EuchreTeam::RemovePoint(int amt)
{
    Points -= amt;
    cout << this->Name << " lost " << to_string(amt) << " point(s)." << endl;

    if (Points < 0) { Points = 0; }

    this->DisplayScore();
}

void EuchreTeam::DisplayScore()
{
    cout << this->Name << ": " << to_string(Points) << " point(s)." << endl;
}

void EuchreBot::DrawCard(Deck &deck)
{
    if (deck.Cards.empty())
    {
        cout << this->Name << " tried to draw a card, but the deck is empty." << endl;
    }
    else
    {
        this->Cards.push_back(deck.Cards.top());
        deck.Cards.pop();
        cout << this->Name << " drew the " << this->Cards.back().Name << " of " << this->Cards.back().Suit << "s." << endl;
    }
}

string Deck::GetOppositeSuit(string suit)
{
    if (suit == "Spade")
    {
        return "Club";
    }
    else if (suit == "Club")
    {
        return "Spade";
    }
    else if (suit == "Heart")
    {
        return "Diamond";
    }
    else
    {
        return "Heart";
    }
}

void Deck::Initialize()
{
    string Suit = "Club";
    string Name = "Ace";
    int Loops = 0;
    for (int i = 14; i > this->LowestNumber - 1 && Loops < 4; i--)
    {
		switch (i)
		{
		case 14:
			Name = "Ace";
            break;
		case 13:
			Name = "King";
            break;
		case 12:
			Name = "Queen";
            break;
		case 11:
			Name = "Jack";
            break;
		default:
			Name = to_string(i);
            break;
		}

        this->Cards.push(Card(Suit, Name, i));

#if defined DEBUG
        cout << "Added the " << this->Cards.top().Name << " of " << this->Cards.top().Suit << "s to the deck." << endl;
#endif

        if (i == this->LowestNumber && Loops < 3)
        {
            switch (Loops)
            {
            case 0:
                Suit = "Spade";
                break;
            case 1:
                Suit = "Diamond";
                break;
            case 2:
                Suit = "Heart";
                break;
            }

            i = 15;
            Loops++;
        }
    }
}

void Deck::Shuffle()
{
    vector<Card> AllCards = vector<Card>();

    while (!this->Cards.empty())
    {
        AllCards.push_back(this->Cards.top());
        this->Cards.pop();
    }

    for (int i = 0; i < AllCards.size(); i++)
    {
        Card temp = AllCards[i];
        srand((unsigned int)time(NULL));
        int otherSlot = i + rand() % (AllCards.size() - i);
        Card otherCard = AllCards[otherSlot];
        AllCards[i] = otherCard;
        AllCards[otherSlot] = temp;
    }

    for (int i = 0; i < AllCards.size(); i++)
    {
        this->Cards.push(AllCards[i]);
#if defined DEBUG
        cout << "The next card in the deck is the " << this->Cards.top().Name << " of " << this->Cards.top().Suit << "s." << endl;
#endif
    }
}

void Deck::AddCard(Card card)
{
    this->Cards.push(card);
}

void Game::GivePoints(EuchreTeam& team, int amt)
{
    team.AddPoints(amt);

    if (team.Points >= WinningScore)
    {
        EndGame(team);
    }
}

void Game::ReturnAllCards()
{
    for (int i = 0; i < Players.size(); i++)
    {
        while (!Players[i].Cards.empty())
        {
            deck.AddCard(Players[i].Cards.back());
            Players[i].Cards.pop_back();
        }
    }
}

void Game::DealCards()
{
    ReturnAllCards();
    deck.Shuffle();

    int CardsToDeal = 1;
    if (TurnCounter < 3) { CardsToDeal = TurnCounter == 1 ? 2 : 3; }

    for (int i = 0; i < Players.size(); i++)
    {
        for (int j = 0; j < CardsToDeal; j++) { Players[i].DrawCard(deck); }
    }
}

void Game::DetermineWin(vector<PlayedCard> CardsPlayed, string LeadingSuit)
{
    EuchreTeam WinningTeam;

    vector<PlayedCard> AllCards = GetMatchingSuits(CardsPlayed, deck.Trump);
    if (AllCards.empty())
    {
        AllCards = GetMatchingSuits(CardsPlayed, LeadingSuit);
        if (AllCards.empty())
        {
            AllCards = CardsPlayed;
        }
    }

    WinningTeam = GetLargestCard(AllCards).Player.Team;

    if (WinningTeam.Name == RedTeam.Name)
    {
        GivePoints(RedTeam, 1);
        if (BlueTeam.AcceptedTrumpThisRound)
        {
            BlueTeam.RemovePoint(1);
        }
    }
    else
    {
        GivePoints(BlueTeam, 1);
        if (RedTeam.AcceptedTrumpThisRound)
        {
            RedTeam.RemovePoint(1);
        }
    }
}

void Game::StartTurn()
{
    cout << endl << "START OF TURN " << TurnCounter << ":" << endl << endl;
    RedTeam.AcceptedTrumpThisRound = false;
    BlueTeam.AcceptedTrumpThisRound = false;

    DealCards();

    deck.Trump = deck.Cards.top().Suit;
    cout << "The trump is " << deck.Trump << "." << endl;

    bool TrumpCalled = false;
    for (int i = 0; i < Players.size() && !TrumpCalled; i++)
    {
        TrumpCalled = Players[i].CallTrump(deck);
    }

    if (!TrumpCalled)
    {
        cout << endl << "Nobody called the trump; returning all cards to the deck and starting a new round." << endl;
    }
    else
    {
        string LeadingSuit = "";
        vector<PlayedCard> CardsPlayed = vector<PlayedCard>();
        vector<Card> MatchingCards = vector<Card>();
        for (int i = 0; i < Players.size(); i++)
        {
            Card TargetCard = Card();

            if (i == 0) //First player in the round plays whatever card they want.
            {
                MatchingCards = GetMatchingSuits(Players[i].Cards, deck.Trump);

                if (MatchingCards.empty()) //The first player has no cards which match the trump, play their largest card by default.
                {
                    TargetCard = GetLargestCard(Players[i].Cards);
                }
                else //The player has at least one trump card, play the largest one they have.
                {
                    TargetCard = GetLargestCard(MatchingCards);
                }

                CardsPlayed.push_back(PlayedCard(Players[i], TargetCard));
                LeadingSuit = CardsPlayed.back().CardPlayed.Suit;
                cout << "The leading suit is " << LeadingSuit << "." << endl;
            }
            else
            {
                MatchingCards = GetMatchingSuits(Players[i].Cards, LeadingSuit);

                if (MatchingCards.empty()) //This player has no cards which match the leading suit.
                {
                    MatchingCards = GetMatchingSuits(Players[i].Cards, deck.Trump);
                    if (MatchingCards.empty()) //They also have no cards which match the trump. Play their largest card by default.
                    {
                        TargetCard = GetLargestCard(Players[i].Cards);
                    }
                    else //They have at least one card which matches the trump, play the largest one they have.
                    {
                        TargetCard = GetLargestCard(MatchingCards);
                    }
                }
                else //They have at least one card which matches the leading suit, play the largest one they have.
                {
                    TargetCard = GetLargestCard(MatchingCards);
                }

                CardsPlayed.push_back(PlayedCard(Players[i], TargetCard));
            }

            cout << Players[i].Name << " played the " << CardsPlayed.back().CardPlayed.Name << " of " << CardsPlayed.back().CardPlayed.Suit << "s." << endl;
        }

        DetermineWin(CardsPlayed, LeadingSuit);
    }

    if (!GameOver)
    {
        TurnCounter++;
        StartTurn(); 
    }
}

void Game::EndGame(EuchreTeam team)
{
    cout << endl << "The game is over. " << team.Name << " wins!" << endl << "The final score was as follows:" << endl;
    RedTeam.DisplayScore();
    BlueTeam.DisplayScore();

    GameOver = true;
}

int main()
{
    Game game = Game(8, 10);

#if defined RUN_NOTES
    //Notes from tutor session below, remove these from this code and put them in a txt before submitting:
    int Array[5];
    int OtherArray[] = { 0, 1, 2, 3, 4 };

    int Added = Add(1, 2);

    vector<int> TestVector = vector<int>();
    TestVector.push_back(1);

    cout << "Vector contains " << TestVector[0] << std::endl;

    TestClass tester = TestClass();

    cout << "Test class is " << tester.TrueOrFalse << std::endl;
    tester.SetTrueOrFalse(true);
    cout << "Now it's  " << tester.TrueOrFalse << std::endl;

    int* TestPointer;   //Creates a pointer.
    //*TestPointer = 8;   //Retrieves the value stored at that pointer.

    int TestInt = 90;
    TestPointer = &TestInt; //TestPointer now points at the address of TestInt.

    cout << "TestInt is " << TestInt << std::endl;
    cout << "TestPointer points to address: " << TestPointer << std::endl;
    cout << "TestInt's address is: " << &TestInt << std::endl;
    *TestPointer = 77;
    cout << "TestInt is now equal to: " << TestInt << std::endl;

    string MyMessage = "Test Message";
    string* MyMessagePointer = &MyMessage;
    tester.PassByRef(MyMessagePointer);
    
    //& means address
    //* means value
    //Passing by reference passes the address of a thing into a function.
    //Passing by value simply passes a copy.
#endif

    return 0;
}