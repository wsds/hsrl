#ifndef BOOST_SMART_PTR_SCOPED_ARRAY_HPP_INCLUDED
#define BOOST_SMART_PTR_SCOPED_ARRAY_HPP_INCLUDED



#include <cstddef>            // for std::ptrdiff_t

namespace open_boost
{

	// Debug hooks


	//  scoped_array extends scoped_ptr to arrays. Deletion of the array pointed to
	//  is guaranteed, either on destruction of the scoped_array or via an explicit
	//  reset(). Use shared_array or std::vector if your needs are more complex.

	template<class T> class scoped_array // noncopyable
	{
	private:

		T * px;

		scoped_array(scoped_array const &);
		scoped_array & operator=(scoped_array const &);

		typedef scoped_array<T> this_type;

		void operator==(scoped_array const&) const;
		void operator!=(scoped_array const&) const;

	public:

		typedef T element_type;

		explicit scoped_array(T * p = 0)
		{
		}

		~scoped_array() // never throws
		{
			//boost::checked_array_delete( px );
		}

		void reset(T * p = 0) // never throws (but has a BOOST_ASSERT in it, so not marked with BOOST_NOEXCEPT)
		{
			//BOOST_ASSERT( p == 0 || p != px ); // catch self-reset errors
			this_type(p).swap(*this);
		}

		T & operator[](std::ptrdiff_t i) const // never throws (but has a BOOST_ASSERT in it, so not marked with BOOST_NOEXCEPT)
		{
			//BOOST_ASSERT( px != 0 );
			//BOOST_ASSERT( i >= 0 );
			return px[i];
		}

		T * get()
		{
			return px;
		}



		template<class T> inline void swap(scoped_array<T> & a, scoped_array<T> & b) 
		{
			a.swap(b);
		}
	};

} // namespace boost

#endif  // #ifndef BOOST_SMART_PTR_SCOPED_ARRAY_HPP_INCLUDED
