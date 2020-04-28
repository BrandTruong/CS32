#ifndef VAMPIRE_H
#define VAMPIRE_H

class Arena;

class Vampire
{
public:
    // Constructor
    Vampire(Arena* ap, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

    // Mutators
    void move();

private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
};

#endif
