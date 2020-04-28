#include <iostream>
#include <string>
using namespace std;
/*
Your declarations and implementations would go here
Event, BasketballGame, Concert, and HockeyGame classes
*/
class Event {
public:
    Event(string name) : m_name(name) {
    }
    virtual ~Event() {
    }
    virtual bool isSport() const {
        return true;
    }
    string name() const {
        return m_name;
    }
    virtual string need() const = 0;
private:
    string m_name;
};
class BasketballGame : public Event {
public:
    BasketballGame(string name) : Event(name) {
    }
    virtual ~BasketballGame() {
        cout << "Destroying the " << name() << " basketball game" << endl;
    }
    string need() const {
        return "hoops";
    }
};
class Concert : public Event {
public:
    Concert(string name, string type) : Event(name), m_type(type) {
    }
    virtual ~Concert() {
        cout << "Destroying the " << name() << " " << m_type << " concert" << endl;
    }
    bool isSport() const {
        return false;
    }
    string need() const {
        return "a stage";
    }
private:
    string m_type;
};
class HockeyGame : public Event {
public:
    HockeyGame(string name) : Event(name) {
    }
    virtual ~HockeyGame() {
        cout << "Destroying the " << name() << " hockey game" << endl;
    }
    string need() const {
        return "ice";
    }
};

void display(const Event* e)
{
    cout << e->name() << ": ";
    if (e->isSport())
        cout << "(sport) ";
    cout << "needs " << e->need() << endl;
}

int main()
{
    Event* events[4];
    events[0] = new BasketballGame("Lakers vs. Suns");
    // Concerts have a name and a genre.
    events[1] = new Concert("Banda MS", "banda");
    events[2] = new Concert("KISS", "hard rock");
    events[3] = new HockeyGame("Kings vs. Flames");
    cout << "Here are the events." << endl;
    for (int k = 0; k < 4; k++) {
        display(events[k]);
    }
    // Clean up the events before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++) {
        delete events[k];
    }
}

/*
Output produced :

Here are the events.
Lakers vs. Suns: (sport) needs hoops
Banda MS: needs a stage
KISS: needs a stage
Kings vs. Flames: (sport) needs ice
Cleaning up.
Destroying the Lakers vs. Suns basketball game
Destroying the Banda MS banda concert
Destroying the KISS hard rock concert
Destroying the Kings vs. Flames hockey game
*/