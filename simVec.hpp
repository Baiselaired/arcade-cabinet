#pragma once

template <class T>
class simVec {
    private:
        T * items;
        int _size;
        int capacity;
        
        void deepCopy(const simVec & rhs) {
            items = new T[capacity];
            _size = rhs._size;
            capacity = rhs.capacity;

            for (int i = 0; i < capacity; i++) {
                items[i] = rhs.items[i];
            }
        }

        void makeEmpty() {delete [] items; _size = 0;}

        void expand() {
            capacity *= 2;
            T * temp = new T[capacity];

            for (int i = 0; i < _size; i++) {
                temp[i] = items[i];
            }
            
            delete [] items;
            items = temp;
        }


    public:
        simVec(int initSize = 2) : capacity(initSize), _size(0) {items = new T[capacity];}
        
        int size() const {return _size;}
        int length() const {return _size;}
        int maxSize() const {return capacity;}

        bool contains(const T & element) const {
            for (int i = 0; i < _size; i++){
                if (items[i] == element){
                    return true;
                }
            }

            return false;
        }

        void append(const T & rhs){
            if (_size == capacity) expand();

            items[_size] = rhs;
            _size++;
        }

        bool erase(T & target){
            if (_size == 0) return false;

            bool found = false;

            for (int i = 0; i < _size; i++){
                if (!found && items[i] == target){
                    delete items[i];
                    found = true;
                    continue;
                }

                if (found && i != (_size)){
                    items[i - 1] = items[i];
                }
            }

            if (found){
                _size--;
                return true;
            }

            return false;
        }

        simVec(const simVec & rhs) {deepCopy(rhs);}

        simVec<T> & operator = (const simVec & rhs){
            if (this == & rhs) return (*this);
            makeEmpty();
            deepCopy(rhs);
			return(*this);
        }

        T & operator [] (int index) const {return items[index];}

        ~simVec(){makeEmpty();}
};