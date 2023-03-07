#include "Person.hpp"
#include <string>


Person::Person() {

}

std::string Person::getFirstName() {
    return firstName;
}

std::string Person::getMiddleName() {
    return middleName;
}

std::string Person::getLastName() {
    return lastName;
}