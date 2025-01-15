template <typename Vector>
class DrainingIterator {
public:
    using iterator = typename Vector::iterator;
    using Value = std::pair<const typename Vector::Key, typename Vector::Value>;
    using BasisVector = typename Vector::BasisVector;

private:
    iterator current;
    Vector* vec;

public:
    DrainingIterator(iterator it, Vector* vec) : current(it), vec(vec) {}

    Value& operator*() {
        return *current;
    }

    Value* operator->() {
        return &(*current); // Return pointer to the underlying pair
    }

    DrainingIterator& operator++() {
        if (current != vec->end()) {
            auto next = std::next(current);
            vec->erase(current);
            current = next;
        }
        return *this;
    }

    DrainingIterator operator++(int) {
        DrainingIterator temp = *this;
        ++(*this);
        return temp;
    }

    // Comparison operators
    bool operator==(const DrainingIterator& other) const {
        return current == other.current;
    }

    bool operator!=(const DrainingIterator& other) const {
        return !(*this == other);
    }
};