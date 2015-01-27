#include <QString>
#include <QtTest>

class FunctionTemplatesTest : public QObject
{
    Q_OBJECT

private slots:

    void testMax();
    void testMaxDifferentArgumentTypes();
    void testMaxVariableReturnType();

    void testCase1_data();
    void testCase1();
};

template <typename T> // OK, providing an extra template arg here without using
                      // it in the call parameters makes argument deduction fail
inline const T &max(const T &a, const T &b)
{
    return a > b ? a : b;
}

void FunctionTemplatesTest::testMax()
{
    QCOMPARE(::max(1, 2), 2);
    QCOMPARE(2, ::max(1, 2));
    QCOMPARE(2.0, ::max(1.0, 2.0));


//    QCOMPARE(2.0, ::max(1, 2.0)); // template argument deduction fails (T = int VS T = double)
    QCOMPARE(2.0, ::max(static_cast<double>(1), 2.0)); // OK, resolved by static cast
    QCOMPARE(2.0, ::max<double>(1, 2.0)); // OK, resolved by providing template type explicitly
    // NOTE: No automatic/implicit type conversation happens for argument deduction


    // The next one is ambiguous due to argument dependend lookup (koenig).
    // There is already std::max.
//    QCOMPARE(max(std::string("a"), std::string("b")), std::string("b"));


    // Make the max definition a declaration, and you will get:
    /*
      warning: inline function 'const T& max(const T&, const T&) [with T =
      double]' used but never defined
     */
}

template <typename T1, typename T2>
// NOTE: Return type is not reference (&) anymore, since type conversation
// (e.g. int to float) can occurr, which produces a temporary objects, for
// which references aren't allowd. inline const T1
T1 max2(const T1 &a, const T2 &b)
{
    return a > b ? a : b;
}

void FunctionTemplatesTest::testMaxDifferentArgumentTypes()
{
    ::max2(1, 2);
    QCOMPARE(::max2(3.3, 2), 3.3);
    QCOMPARE(::max2(2, 3.3), 3); // Uh! return types differs, depends on first argument!
}

// OK, do not specify return type explicitly by using decltype
template <typename T1, typename T2>
auto max3(const T1 &a, const T2 &b) -> decltype(a > b ? a : b) // OK, return more precise type
{
    return a > b ? a : b;
}


void FunctionTemplatesTest::testMaxVariableReturnType()
{
    QCOMPARE(::max3(1, 2.0), 2.0);
    QCOMPARE(::max3(1.0, 2), static_cast<double>(2));
}

// NOTE: OK, default template arguments for function templates are allowd with >= C++11 not C++98!

void FunctionTemplatesTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

void FunctionTemplatesTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(FunctionTemplatesTest)

#include "tst_functiontemplatestest.moc"
