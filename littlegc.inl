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

				T& operator *() {
					assert(_ptr&&"_ptr is a NULL pointer");
					return *_ptr;
				}
				const T& operator *() const {
					assert(_ptr&&"_ptr is a NULL pointer");
					return *_ptr;
				}

				T* operator -> () {
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
		using lgc = LittleGC<T>;
}
namespace lgc_user {
	using little_gc::lgc;
	using little_gc::gc;
}
#endif
