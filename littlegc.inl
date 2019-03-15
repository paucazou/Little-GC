#ifndef LITTLE_GC_H
#define LITTLE_GC_H
/* This file contains a draft
 * of a Garbage Collector
 * called Little GC
 */

#include "referencer.inl"
#include <cassert>

namespace little_gc {
	template <typename T>
		class LittleGC
		{
			public:
				LittleGC (const T& elt) :
					_ref{getReferencer<T>()},
					_id{_ref.firstTimeConstruction(elt)},
					_ptr{_ref.getPtr(_id)}
				{
				}

				LittleGC (const T&& elt) :
					_ref { getReferencer<T>() },
					_id {_ref.firstTimeConstruction(std::move(elt))},
					_ptr{_ref.getPtr(_id)}
				{
				}

				LittleGC (const LittleGC<T>& other) :
					 _ref{other._ref},
					_id{other._id},
					_ptr{other._ptr}
				{
					_ref.notifyCopy(_id);
				}

				~LittleGC() {
					_ref.notifyDestruction(_id);
				}

				LittleGC<T>& operator = (const LittleGC<T>& other) {
					/* Increments the references count by one
					 * for other, but decrements it for this before
					 * copy
					 */
					if (other == *this)
						return *this;
					// save the old id in a temporary variable
					auto old_id { _id };
					// copy data references
					_id = other._id;
					_ptr = other._ptr;
					// no need to copy _ref since it is supposed
					// to be the same for other
					
					// increments the current data count
					// and decrements the old one. We do that here
					// for the case other points actually to the same data
					_ref.notifyCopy(_id);
					_ref.notifyDestruction(old_id);
					return *this;
				}

				LittleGC<T>& operator = (const LittleGC<T>&& other) {
					/* Move assignment. See copy assignment for details
					 */
					if (other == *this)
						return *this;

					auto old_id { _id };
					_id = std::move(other._id);
					_ptr = std::move(other._ptr);

					_ref.notifyCopy(_id);
					_ref.notifyDestruction(old_id);
					return *this;
				}

				T& operator *() {
					assert(_ptr&&"_ptr is a NULL pointer");
					return *_ptr;
				}
				const T& operator *() const {
					assert(_ptr&&"_ptr is a NULL pointer");
					return *_ptr;
				}

				bool operator == (const LittleGC<T>& other) const{
					/* true if other points to the same data
					 */
					return other._id == _id && other._ptr == _ptr;
				}

				bool operator != (const LittleGC<T>& other) const{
					return !(other == *this);
				}

				T* operator -> () const{
					assert(_ptr&&"_ptr is a NULL pointer");
					return _ptr;
				}
			private:
				Referencer<T>& _ref;
				unsigned int _id{0};
				T* _ptr{nullptr};
		};

	template <typename T>
		const LittleGC<T> gc(const T& elt) {
			/* Creates a garbage-collected variable
			 * No need to specify the template-arg
			 * with this one
			 */
			return LittleGC<T>(elt);
		}
	template <typename T>
		const LittleGC<T> gc(const T&& elt) {
			/* Overloaded move factory
			 */
			return LittleGC<T>(std::move(elt));
		}

	template <typename T>
		const LittleGC<T> gc() {
			/* Overloaded factory. Calls the default
			 * constructor
			 */
			return LittleGC<T>( T() );
		}

	template <typename T>
		using lgc = LittleGC<T>;
}
namespace lgc_user {
	using little_gc::lgc;
	using little_gc::gc;
}
#endif
