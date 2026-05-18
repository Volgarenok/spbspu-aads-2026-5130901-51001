#include "calc.cpp"
#include <cassert>

namespace
{
  void test_stack()
  {
    krivoshapov::Stack<int> s;
    assert(s.empty());
    assert(s.size() == 0);

    s.push(1);
    s.push(2);
    s.push(3);
    assert(s.size() == 3);
    assert(s.top() == 3);

    s.pop();
    assert(s.top() == 2);
    assert(s.size() == 2);

    krivoshapov::Stack<int> c(s);
    c.push(9);
    assert(c.size() == 3 && s.size() == 2);

    krivoshapov::Stack<int> m(std::move(c));
    assert(m.size() == 3 && c.empty());

    krivoshapov::Stack<int> a;
    a = s;
    assert(a.size() == s.size());

    krivoshapov::Stack<int> b;
    b = std::move(m);
    assert(b.size() == 3 && m.empty());

    b.clear();
    assert(b.empty());

    bool threw = false;
    try
    {
      krivoshapov::Stack<int> e;
      e.pop();
    }
    catch (const std::out_of_range &)
    {
      threw = true;
    }
    assert(threw);

    threw = false;
    try
    {
      krivoshapov::Stack<int> e;
      e.top();
    }
    catch (const std::out_of_range &)
    {
      threw = true;
    }
    assert(threw);
  }

  void test_queue()
  {
    krivoshapov::Queue<int> q;
    assert(q.empty());

    for (int i = 0; i < 20; ++i)
    {
      q.push(i);
    }
    assert(q.size() == 20);

    for (int i = 0; i < 10; ++i)
    {
      assert(q.front() == i);
      q.pop();
    }
    assert(q.front() == 10 && q.size() == 10);

    krivoshapov::Queue<int> c(q);
    assert(c.front() == 10 && c.size() == 10);
    c.pop();
    assert(q.front() == 10);

    krivoshapov::Queue<int> m(std::move(c));
    assert(m.front() == 11 && c.empty());

    krivoshapov::Queue<int> a;
    a = q;
    assert(a.size() == q.size());

    krivoshapov::Queue<int> b;
    b = std::move(m);
    assert(!b.empty() && m.empty());

    b.clear();
    assert(b.empty());

    bool threw = false;
    try
    {
      krivoshapov::Queue<int> e;
      e.pop();
    }
    catch (const std::out_of_range &)
    {
      threw = true;
    }
    assert(threw);

    threw = false;
    try
    {
      krivoshapov::Queue<int> e;
      e.front();
    }
    catch (const std::out_of_range &)
    {
      threw = true;
    }
    assert(threw);
  }
}
