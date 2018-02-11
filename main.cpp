class ThisIsAClass
{
  public:
    void method() const;
};

void ThisIsAClass::method() const
{
  // hello
}

int main()
{
  ThisIsAClass a;
  a.method();
  
  return 0;
}
