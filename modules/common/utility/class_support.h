#ifndef CLASS_SUPPORT_H_
#define CLASS_SUPPORT_H_

#define PUBLIC_MEMBER_ASSIGN_METHOD(type, name)          \
 public:                                                 \
  bool has_##name() const { return flag_has_##name##_; } \
  void set_##name(const type& value) {                   \
    flag_has_##name##_ = true;                           \
    name = value;                                        \
  }                                                      \
  type get_##name() const { return name; }               \
                                                         \
  type* get_mutable_##name() { return &name; }           \
                                                         \
 private:                                                \
  bool flag_has_##name##_ = false;

#define PRIVATE_MEMBER(type, name) \
 private:                          \
  type name;                       \
  PUBLIC_MEMBER_ASSIGN_METHOD(type, name)

#define PROTECTED_MEMBER(type, name) \
 protected:                          \
  type name;                         \
  PUBLIC_MEMBER_ASSIGN_METHOD(type, name)

#endif