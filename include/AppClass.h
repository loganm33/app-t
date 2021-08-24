#pragma once

class AppClass {
/** Special */
public:
    AppClass();
    AppClass(const AppClass &other) = default;
    AppClass(AppClass &&other) noexcept = default;
    ~AppClass() = default;
/** Accessors */
public:
    [[nodiscard]] const int &attr() const;
    void set_attr(const int &newAttr);
/** Attributes */
private:
    int attr_;
};
