#ifndef NON_COPUABLE_H_INCLUDED
#define NON_COPUABLE_H_INCLUDED


class NonCopyable
{
public:
    NonCopyable() = default;
    NonCopyable& operator = (const NonCopyable& lv) = delete;
    NonCopyable (NonCopyable& copy)                 = delete;
};


#endif // !NON_COPUABLE_H_INCLUDED
