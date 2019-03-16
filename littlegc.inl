#ifndef LITTLE_GC_H
#define LITTLE_GC_H
/* This file contains a draft
 * of a Garbage Collector
 * called Little GC
 */

#include "data.inl"
#include <cassert>

namespace little_gc {
	template <typename T>
		class LittleGC
		{
			public:
				LittleGC (const T& elt) :
					_ptr { new __underlying_data<T>(elt) }
				{
				}

				LittleGC (const T&& elt) :
					_ptr { new __underlying_data<T>(std::move(elt)) }
				{
				}

				LittleGC (const LittleGC<T>& other) :
					_ptr{other._ptr}
				{
					++(*_ptr);
				}

				~LittleGC() {
					__decrement_ptr();
				}

				LittleGC<T>& operator = (const LittleGC<T>& other) {
					/* Increments the references count by one
					 * for other, but decrements it for this before
					 * copy
					 */
					if (other == *this)
						return *this;
					// save the old ptr in a temporary variable
					auto old_ptr { _ptr};
					// copy data references
					_ptr = other._ptr;
					
					// increments the current data count
					// and decrements the old one. We do that here
					// for the case other points actually to the same data
					++(*_ptr);
					__decrement_ptr(old_ptr);
					return *this;
				}

				LittleGC<T>& operator = (const LittleGC<T>&& other) {
					/* Move assignment. See copy assignment for details
					 */
					if (other == *this)
						return *this;

					auto old_ptr{ _ptr};
					_ptr = std::move(other._ptr);

					++(*_ptr);
					__decrement_ptr(old_ptr);
					return *this;
				}

				LittleGC<T>& operator = (const T& other) {
					/* Equals to *this = LittleGC<T>& other
					 */
					return *this = LittleGC<T>(other);
				}
				
				LittleGC<T>& operator = (const T&& other) {
					/* Equal to *this = LittleGC<T>&& other
					 */
					return *this = LittleGC<T>(std::move(other));
				}

				T& operator *() {
					assert(_ptr&&"_ptr is a NULL pointer");
					return ***_ptr;
				}
				const T& operator *() const {
					assert(_ptr&&"_ptr is a NULL pointer");
					return ***_ptr;
				}

				bool operator == (const LittleGC<T>& other) const{
					/* true if other points to the same data
					 */
					return other._ptr == _ptr;
				}

				bool operator != (const LittleGC<T>& other) const{
					return !(other == *this);
				}

				T* operator -> () const{
					assert(_ptr&&"_ptr is a NULL pointer");
					return **_ptr;
				}
				template <typename... Args>
					static LittleGC<T> __constructs_inplace(Args&&... args)
					{
						/* Call directly the constructor
						 * of T
						 */
						auto empty { LittleGC<T>() };
						auto ptr { new __underlying_data<T>(std::forward<Args>(args)...) };
						empty._ptr = ptr;
						return empty;
					}
			private:
				__underlying_data<T>* _ptr{nullptr};

			private: // functions
				LittleGC<T> () :
					_ptr { nullptr}
				{
				}

				static unsigned int __decrement_ptr(__underlying_data<T>* ptr) {
					/* Decrements the pointer
					 * and deletes it if necessary
					 */
					auto count { --(*ptr) };
					if (count == 0)
						delete ptr;
					return count;
				}
				unsigned int __decrement_ptr() {
					return __decrement_ptr(_ptr);
				}


		};

	template <typename T>
		LittleGC<T> gc(const T& elt) {
			/* Creates a garbage-collected variable
			 * No need to specify the template-arg
			 * with this one
			 */
			return LittleGC<T>(elt);
		}
	template <typename T>
		LittleGC<T> gc(const T&& elt) {
			/* Overloaded move factory
			 */
			return LittleGC<T>(std::move(elt));
		}

	template <typename T>
		LittleGC<T> gc() {
			/* Overloaded factory. Calls the default
			 * constructor
			 */
			return LittleGC<T>( T() );
		}
	template <typename T, typename... Args>
		LittleGC<T> gce(Args&&... args) {
			/* Constructs T in place
			 * without copy
			 */
			return LittleGC<T>::__constructs_inplace(std::forward<Args>(args)...);
		}

	template <typename T>
		using lgc = LittleGC<T>;
}
namespace lgc_user {
	using little_gc::lgc;
	using little_gc::gc;
}
#endif
