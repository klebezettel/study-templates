#include <QString>
#include <QtTest>

#include <stdexcept>

class ClassTemplatesTest : public QObject
{
    Q_OBJECT

private slots:
    void testCase1();
};

// Name and type of the following class:
//  Name: 'Stack'
//  Type: 'Stack<T>'
template <class T>
class Stack {
private:
    std::vector<T> elems;

    Stack(const Stack<T> &other); // OK, type is required here
    Stack<T> &operator=(const Stack<T> &other); // ...as above

public:
    Stack() {} // OK, constructor requires name, not type
    ~Stack() {} // ...as above

    void push(T const &elem)
    {
        elems.push_back(elem);
    }

    T pop();
    T top() const;
};

template <typename T>
T Stack<T>::pop()
{
    if (elems.empty())
        throw std::out_of_range("Stack<>::pop(): empty stack");
    T elem = elems.back();
    elems.pop_back();
    return elem;
}

template <typename T>
T Stack<T>::top() const
{
    if (elems.empty())
        throw std::out_of_range("Stack<>::pop(): empty stack");
    return elems.back();
}

void ClassTemplatesTest::testCase1()
{
    Stack<int> intStack;
    Stack<std::string> stringStack;

    intStack.push(7);
    QCOMPARE(intStack.top(), 7);
    // NOTE: Code ist instantiated only for member functions that are called
    //    - saves time + space
    //    - allows us to instantiate class templates for which the type does
    //      not can perform all member functions
}

QTEST_APPLESS_MAIN(ClassTemplatesTest)

#include "tst_classtemplatestest.moc"
