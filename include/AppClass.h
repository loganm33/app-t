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
    const int &attr() const;
    void setAttr(const int &newAttr);
/** Attributes */
private:
    int attr_;
};
