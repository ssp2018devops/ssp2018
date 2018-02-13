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
  ThisIsAClass* a = new ThisIsAClass();
  a->method();
  
  return 0;
}
