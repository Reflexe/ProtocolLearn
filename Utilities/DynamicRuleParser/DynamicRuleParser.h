/**
 * @file DynamicRuleParser.h
 * @author shrek0 (shrek0.tk@gmail.com)
  DynamicRuleParser
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) 2015 shrek0
 *
 * ProtocolLearn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProtocolLearn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 * Contains a class for a commoon values filtering.
 */

#ifndef DYNAMICRULEPARSER_H
#define DYNAMICRULEPARSER_H

#include <unordered_map>
#include <stack>
#include <list>

#include <cstdint>

namespace ProtocolLearn {

struct FilterOperator{
    enum {                     // In C
        Equal           = 0b00000001U, // ==
        GreaterThan     = 0b00000010U, // >
        LessThan        = 0b00000100U, // <
        Not             = 0b00100000U, // !
        Or              = 0b01000000U, // ||
        And             = 0b10000000U  // &&
    };
};

template<class T>
class DynamicRuleParser
{
public:
    typedef uint8_t FieldIDType;
    typedef uint8_t OperatorType;

    template<class ValueType>
    struct Rule{
        OperatorType operatorType;
        FieldIDType fieldID; // The ID of the value.
        ValueType value;             // The data for the action.;
    };

    typedef Rule<T>  RuleType;
    typedef std::list<RuleType> RulesType;

    explicit DynamicRuleParser(const RulesType &rules) : rules(rules)
    {
    }

    DynamicRuleParser()
    {

    }

    enum class FinalReturnCodes{
        True,
        False,
        Continue
    };

    /**
     * @brief Add value for checking with the rules.
     * @param fieldID  A idenfication number for the value (a.k.a. Key).
     * @param value  A value of the field.
     */
    void addValue(FieldIDType fieldID, T value)
    {
        d.values[fieldID] = value;
    }

    /**
     * @brief Push the required fields IDs to given stack.
     * @param fields  A stack for pushing.
     */
    void getRequiredFieldsIDs(std::stack<FieldIDType> &fields) const{
        std::unordered_map<FieldIDType, bool> ifExist; // Checking for no repeated IDs.

        for(const auto &rule : rules) {
            if(ifExist.count(rule.fieldID) == 0) {
                ifExist[rule.fieldID] = true;
                fields.push(rule.fieldID);
            }
        }
    }

    /**
     * @brief check the given values with the given rules.
     * @return True if the conditions are right, False if not.
     */
    bool check() {
        FinalReturnCodes finalResult = FinalReturnCodes::False;

        if(rules.empty())
            return true;

        for(const auto &rule : rules) {
            finalResult = checkIntermediateOperators(
                        checkRule(rule.fieldID, rule.value, getOnlyArithmeticOperators(rule.operatorType)),
                        getOnlyLogicalOperators(rule.operatorType)
                        );

            if(finalResult == FinalReturnCodes::False)
                return false;
            if(finalResult == FinalReturnCodes::True)
                return true;
        }

        return false;
    }

    void addRules(const RulesType &rules)
    {
        this->rules.insert(this->rules.end(), rules.begin(), rules.end());
    }

    void setRules(const RulesType &rules)
    {
        this->rules = rules;
    }

    RulesType &getRules()
    {
        return rules;
    }

    void clearValues()
    {
        d.values.clear();
    }

    void clearRules()
    {
        rules.clear();
    }

private:    
    /**
     * @brief getValue  Get value by fieldID
     * @param fieldID  A ID number of the requested value.
     * @return the requested value.
     */
    const T &getValue(FieldIDType fieldID)
    {
        return d.values[fieldID];
    }

    bool checkRule(const FieldIDType &fieldID, const T &ruleValue, OperatorType operators) {
        const T value = getValue(fieldID);

        switch(operators){
        case FilterOperator::Equal:
            return equal(value, ruleValue);
            break;
        case FilterOperator::GreaterThan:
            return moreThan(value, ruleValue);
            break;
        case FilterOperator::LessThan:
            return lessThan(value, ruleValue);
            break;
        case FilterOperator::Equal|FilterOperator::GreaterThan:
            return equalOrMoreThan(value, ruleValue);
            break;
        case FilterOperator::Equal|FilterOperator::LessThan:
            return equalOrLessThan(value, ruleValue);
            break;
        default:
            return equal(value, ruleValue);
            break;
        }
    }

    static FinalReturnCodes checkIntermediateOperators(bool result, OperatorType operators) {
        if(result == false) {
            switch(operators) {
            case FilterOperator::Not:
                return FinalReturnCodes::True;
                break;
            case FilterOperator::Not|FilterOperator::Or:
                return FinalReturnCodes::True;
                break;
            case FilterOperator::Not|FilterOperator::And:
                return FinalReturnCodes::Continue;
                break;
            case FilterOperator::And:
                return FinalReturnCodes::False;
                break;
            case FilterOperator::Or:
                return FinalReturnCodes::Continue;
                break;
            case 0:
                return FinalReturnCodes::False;
                break;
            default:
                return FinalReturnCodes::False;
                break;
            }
        }
        else if(result == true) {
            switch(operators) {
            case FilterOperator::Not:
                return FinalReturnCodes::False;
                break;
            case FilterOperator::Not|FilterOperator::Or:
                return FinalReturnCodes::Continue;
                break;
            case FilterOperator::Not|FilterOperator::And:
                return FinalReturnCodes::False;
                break;
            case FilterOperator::And:
                return FinalReturnCodes::Continue;
                break;
            case FilterOperator::Or:
                return FinalReturnCodes::True;
                break;
            case 0:
                return FinalReturnCodes::True;
                break;
            default:
                return FinalReturnCodes::False;
                break;
            }
        }
        return FinalReturnCodes::False;
    }

    /**
     * @brief Get only the Equal, LessThan, MoreThan bit flags from operators value.
     * @param operators  A original value of the given operators.
     * @return clean oprator flags.
     */
    static inline OperatorType getOnlyArithmeticOperators(const OperatorType &operators)
    {
        return operators & 0b0000111;
    }

    /**
     * @brief Get only the And,Or,Not bit flags from operators value.
     * @param operators  A original value of the given operators.
     * @return clean oprator flags.
     */
    static inline OperatorType getOnlyLogicalOperators(const OperatorType &operators)
    {
        return operators & 0b1110000;
    }


    /**
     * @brief Check if arg1 is equal to arg2.
     * @param arg1
     * @param arg2
     * @return arg1==arg2
     */
    static bool equal(const T &arg1, const T &arg2)
    {
        return arg1 == arg2;
    }


    /**
     * @brief Check if arg1 is less than arg2.
     * @param arg1
     * @param arg2
     * @return arg1 < arg2.
     */
    static bool lessThan(const T &arg1, const T &arg2)
    {
        return arg1 < arg2;
    }

    /**
     * @brief Check if arg1 is more than arg2.
     * @param arg1
     * @param arg2
     * @return arg1 > arg2
     */
    static bool moreThan(const T &arg1, const T &arg2)
    {
        return arg1 > arg2;
    }


    /**
     * @brief Check if arg1 is more than or equal to arg2.
     * @param arg1
     * @param arg2
     * @return arg1 >= arg2
     */
    static bool equalOrMoreThan(const T & arg1, const T &arg2)
    {
        return arg1 >= arg2;
    }

    /**
     * @brief Check if arg1 is less than or equal to arg2.
     * @param arg1
     * @param arg2
     * @return arg1 <= arg2
     */
    static bool equalOrLessThan(const T & arg1, const T &arg2)
    {
        return arg1 <= arg2;
    }

    /**
     * Conatins the filter rules.
     */
    RulesType rules;

    struct Data{
        std::unordered_map<FieldIDType, T> values;
    };

    /**
     * @brief d Contain the values.
     */
    Data d;
};

}

#endif // DYNAMICRULEPARSER_H
