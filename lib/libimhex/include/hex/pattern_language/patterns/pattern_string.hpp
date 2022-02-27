#pragma once

#include <hex/pattern_language/patterns/pattern.hpp>

namespace hex::pl {

    class PatternString : public Pattern {
    public:
        PatternString(Evaluator *evaluator, u64 offset, size_t size, u32 color = 0)
            : Pattern(evaluator, offset, size, color) { }

        [[nodiscard]] std::unique_ptr<Pattern> clone() const override {
            return std::unique_ptr<Pattern>(new PatternString(*this));
        }

        void createEntry(prv::Provider *&provider) override {
            auto size = std::min<size_t>(this->getSize(), 0x7F);

            if (size == 0)
                return;

            std::vector<u8> buffer(size, 0x00);
            provider->read(this->getOffset(), buffer.data(), size);

            auto displayString = hex::encodeByteString(buffer);
            this->createDefaultEntry(hex::format("\"{0}\" {1}", displayString, size > this->getSize() ? "(truncated)" : ""), displayString);
        }

        [[nodiscard]] std::string getFormattedName() const override {
            return "String";
        }

        [[nodiscard]] std::string toString(prv::Provider *provider) const override {
            std::string buffer(this->getSize(), 0x00);
            provider->read(this->getOffset(), buffer.data(), buffer.size());

            std::erase_if(buffer, [](auto c) {
                return c == 0x00;
            });

            return buffer;
        }

        [[nodiscard]] bool operator==(const Pattern &other) const override { return areCommonPropertiesEqual<decltype(*this)>(other); }
    };

}