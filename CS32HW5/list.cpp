void listAll(string path, const Class* c)  // two-parameter overload
{
    string s = path + c->name();
    cout << s <<endl;
    s += "=>";
    for (vector<Class*>::const_iterator it = c->subclasses().cbegin(); it != c->subclasses().cend(); it++) { //traverse through vector
        listAll(s, *it);
    }
}