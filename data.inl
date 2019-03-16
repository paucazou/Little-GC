#ifndef DATA_GC_H
#define DATA_GC_H
#include <utility>

namespace little_gc {
	template <typename T>
		class __underlying_data {
			/* This class represents the underlying
			 * data used by the Garbage Collector
			 */
			public:
				template <typename... Args>
					__underlying_data<T> (Args&&... args) :
						_data { std::forward<Args>(args)... },
						_count { 1 } 
				{
					/* Unique constructor
					 */
				}

				T* operator *() {
					return &_data;
				}
				
				unsigned int operator ++ () {
					/* Increments the
					 * count number
					 */
					return _count++;
				}

				unsigned int operator -- () {
					/* Decreases the
					 * count number
					 * and destroys
					 * the underlying data
					 * if _count == 0
					 */
					return --_count;
				}
				__underlying_data<T>& operator = (const __underlying_data<T>&) = delete;

			private:
				T _data;
				unsigned int _count{1};
		};
}
#endif
