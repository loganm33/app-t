#pragma once

class AppClass {
public:  /**< Special */
    AppClass();
    AppClass(const AppClass &other) = default;
    AppClass(AppClass &&other) noexcept = default;
    ~AppClass() = default;
public:  /**< Accessors */
    const int &attr() const;
    void setAttr(const int &newAttr);
private:  /**< Attributes */
    int attr_;
};
