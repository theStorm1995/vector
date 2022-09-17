// Name: Nathan Storm 
// CWID: 885905158
// Email: nathanstorm95@csu.fullerton.edu

#include <assert.h>
#include <iostream>
#include <string>


//
template <typename T>
class MyVector
{
	public:
		
		/*******************
		 * Static constants
		 ******************/
		
		/// Default capacity
		static constexpr size_t DEFAULT_CAPACITY = 64;
		
		/// Minimum capacity
		static constexpr size_t MINIMUM_CAPACITY = 8;
		
		/*****************************
		 * Constructors / Destructors
		 ****************************/
		
		/// Normal constructor
		MyVector(size_t capacity = MyVector::DEFAULT_CAPACITY) {
			capacity_ = capacity;
			if (capacity_ < MINIMUM_CAPACITY) {
				capacity_ = MINIMUM_CAPACITY;
			}
			elements_ = new T[capacity_];
		}
		
		/// Copy constructor
		MyVector(const MyVector& other) {
			capacity_ = other.capacity_;
			size_ = other.size_;
			copyOther(other);
		}
		
		/**
		 * Destructor
		 * Should call clear() so each element gets its destructor called.
		 * Then, deallocate the internal array and make it a nullptr, if its not already a nullptr.
		 */
		~MyVector() {
			clear();
			delete [] elements_;
			elements_ = nullptr;
		}
		
		/************
		 * Operators
		 ************/
		
		///	Assignment operator
		MyVector& operator=(const MyVector& rhs) {
			capacity_ = rhs.capacity_;
			size_ = rhs.size_;
			if (this == &rhs){
				return *this;
			}
			delete [] elements_;
			copyOther(rhs);
			return *this;
		}
		
		/// Operator overload to at()
		T& operator[](size_t index) const {
			assertBounds(index, "Out of Range");
			return *(elements_ + index);
		}
		
		/************
		 * Accessors
		 ************/
		
		/// Return the number of valid elements in our data
		size_t size() const {
			return size_;
		}
		
		/// Return the capacity of our internal array
		size_t capacity() const {
			return capacity_;
		}
		
		/**
		 * Check whether our vector is empty
		 * Return true if we have zero elements in our array (regardless of capacity)
		 * Otherwise, return false
		 */
		bool empty() const {
			if (size_ == 0) {
				return true;
			}
			else {
				return false;
			}
		}
		
		/// Return a reference to the element at an index
		T& at(size_t index) const {
			return operator[](index);
		}
		
		/***********
		 * Mutators
		 ***********/
		
		/**
		 * Reserve capacity in advance, if our capacity isn't currently large enough.
		 * Useful if we know we're about to add a large number of elements, and we'd like to avoid the overhead of many internal changes to capacity.
		 */
		void reserve(size_t capacity) {
			if(capacity > capacity_){
				changeCapacity(capacity);
			}
		}
		
		/**
		 * Set an element at an index.
		 * Throws range error if outside the size boundary.
		 * Returns a reference to the newly set element (not the original)
		 */
		T& set(size_t index, const T& element) {
			assertBounds(index, "Index is out of range.");
			this->elements_[index].~T();
			elements_[index] = element;
			return elements_[index];
		}
		
		/**
		 * Add an element onto the end of our vector, increasing the size by 1
		 * Should rely on the insert() function to avoid repeating code.
		 * Returns a reference to the newly inserted element
		 */
		T& push_back(const T& element) {
			return insert(size_, element);
		}
		
		/**
		 * Remove the last element in our vector, decreasing the size by 1
		 * Should rely on the erase() function to avoid repeating code.
		 * Returns the new size.
		 */
		size_t pop_back() {
			size_t last_index = size_ - 1;
			return erase(last_index);
		}
		
		/**
		 * Insert an element at some index in our vector, increasing the size by 1
		 * Returns a reference to the newly added element (not the original).
		 */
		T& insert(size_t index, const T& element) {
			if(index > size_) {
				throw std::range_error("Out of Range");
			}
			increaseSize();

			// Move each element 1 position to the right. 
			for (size_t i = index; i < size_; i++){
				elements_[i + 1] = elements_[i];
			}
			return set(index, element);
		}
		
		/**
		 * Erase one element in our vector at the specified index, decreasing the size by 1
		 * Throws std::range_error if the index is out of bounds.
		 * Calls the erased element's destructor.
		 * Returns the new size.
		 */
		size_t erase(size_t index) {
			// Check index is out of bounds
			assertBounds(index, "Out of Range");

			// Erase element at index
			for(size_t i = index; i < (size_ - 1); i++) {
				elements_[i] = elements_[i + 1];
			}

			// Call the last elements destructor
			this->elements_[index].~T();

			// Decrease size by 1
			decreaseSize();
			return size_;
		}
		
		/**
		 * Calls each element's destructor, then clears our internal
		 * data by setting size to zero and resetting the capacity.
		*/
		void clear() {
			if(size_ > 0) {
				pop_back();
			}
			capacity_ = 0;
			size_ = 0;
		}
	
	/**
	 * Begin private members and methods.
	 * Private methods can often contain useful helper functions,
	 * or functions to reduce repeated code.
	*/
	private:
		
		/// Number of valid elements currently in our vector
		size_t size_ = 0;
		
		/// Capacity of our vector; The actual size of our internal array
		size_t capacity_ = 0;
		
		/**
		 * Our internal array of elements of type T.
		 * Starts off as a null pointer.
		 */
		T* elements_ = nullptr;
		
		/**
		 * Increases the size of our vector by 1 element.
		 * If more capacity is needed, it is doubled.
		 */
		void increaseSize() {
			if (size_ + 1 > capacity_) {
				capacity_ = capacity_ * 2;
				changeCapacity(capacity_);
			}
			size_++;
		}
		
		/**
		 * Decreases the size of our vector by 1 element.
		 * If our size is less than a third of our capacity, cuts our capacity in half.
		 * Capacity should always be at least MyVector::MINIMUM_CAPACITY
		 */
		void decreaseSize() {
			size_t one_third = capacity_ / 3;
			size_t half_cap = capacity_ / 2;
			if (size_ < one_third) {
				if ((half_cap) > MyVector::MINIMUM_CAPACITY) {
					changeCapacity(half_cap);
				}
				else {
					changeCapacity(MyVector::MINIMUM_CAPACITY);
				}
			}
			size_--;
		}
		
		/**
		 * Helper function that is called whenever we need to change the capacity of our vector
		 * Should throw std::range_error when asked to change to a capacity that cannot hold our existing elements.
		 */
		void changeCapacity(size_t c) {
			// Check if new capacity can hold our elements
			assertCapacity(c);
			capacity_ = c;
			copyElements(elements_, size_);
		}
		
		/**
		 * Copy another vector's elements into our own, by value.
		 * Does not simply copy the other vector's array's pointer
		 * Does not care about matching the capacity exactly.
		 * This is a helper function relied upon by the copy constuctor and the assignment operator
		 */
		void copyOther(const MyVector& other) {
			elements_ = new T[other.size()];
			for(size_t i = 0; i < other.size(); i++) {
				elements_[i] = other[i];
			}
		}
		
		/**
		 * Receives a pointer to an array of elements, and their size.
		 * Throw an std::range_error if we don't already have enough capacity.
		 * Then, create a new temporary array, and copy each element by value.
		 * Next, delete our internal array if it wasn't a null pointer.
		 * Next, assign the new temporary array pointer to our internal array pointer.
		 * Finally, change our size attribute to match.
		 */
		void copyElements(T* pElements, size_t size) {
			// Bounds check to make sure we have enough capacity 
		    assertCapacity(capacity_);

			// Create temp array of new capacity 
			T* temp = new T[capacity_];

			// Copy Each element from pElements into temp
			for(size_t i = 0; i < size; i++) {
				temp[i] = pElements[i];
			}

			// Make our internal pointer array point to the temp array
			if (elements_ != nullptr) {
				delete [] elements_;
				elements_ = nullptr;
			}
			elements_ = temp;
			temp = nullptr;
		}
		
		//
		void assertCapacity(size_t c) const {
			if(c < size_){
				throw std::range_error("Size is bigger than capacity");
			}
		}
		
		//
		void assertBounds(size_t index, std::string message = "") const {
			if((index > size_ - 1) || index < 0 || size_ == 0) {
				throw std::range_error(message);
			}
		}
};



