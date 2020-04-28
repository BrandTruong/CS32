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