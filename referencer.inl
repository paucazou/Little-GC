#ifndef REFERENCER_H
#define REFERENCER_H
#include <queue>
#include <utility>
#include <vector>

namespace little_gc {
	template <typename T>
		class Referencer {
			/* Singleton that contains all the references
			 * to the variables of type T
			 */
			using ptr_index = std::pair<T*,unsigned int>;

			public:
				static Referencer<T>& getReferencer() {
					return _instance;
				}

				void notifyCopy(unsigned int i) {
					/* Notifies the referencer
					 * that a copy of the item
					 * of index i has been made
					 */
					_references.at(i).second++;
				}

				void notifyDestruction(unsigned int i) {
					/* Notifies the referencer
					 * that a reference of the item
					 * of index i has been removed
					 */
					if (--_references.at(i).second == 0) {
						delete _references.at(i).first;
						_references.at(i).first = nullptr;
						_empty_references.push(i);
					}
				}

				unsigned int firstTimeConstruction(const T& elt) {
					/* Allocates elt on the heap and return
					 * the index of the pointer
					 */
					unsigned int index { getNextIndex() };
					T* ptr { new T(elt) };
					constexpr unsigned int ref_number {1};
					_references.at(index) = ptr_index(ptr,ref_number);
					
					return index;
				}


				T* getPtr(unsigned int i) {
					/* Returns the pointer matching with
					 * index i
					 */
					return _references.at(i).first;
				}

			private:
				static Referencer<T> _instance;
				std::vector<std::pair<T*,unsigned int>> _references;
				std::queue<unsigned int> _empty_references;

				unsigned int getNextIndex() {
					/* Return the next index of the references
					 * that is empty
					 */
					unsigned int index {0};
					if (!_empty_references.empty()) {
						index = _empty_references.front();
						_empty_references.pop();
					} else {
						_references.emplace_back(nullptr,0);
						index = _references.size() - 1;
					}
					return index;
				}
		};

	template <typename T>
		Referencer<T>& getReferencer() {
			/* Return a referencer of the correct
			 * type
			 */
			static Referencer<T> _referencer;
			return _referencer;
		}
}
#endif
