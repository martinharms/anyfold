#ifndef _IMAGE_STACK_UTILS_H_
#define _IMAGE_STACK_UTILS_H_
#include <vector>
#include <iostream>
#include <iomanip>
#include "boost/multi_array.hpp"


namespace anyfold {

  typedef  boost::multi_array<float,              3>    image_stack;
  typedef  boost::multi_array_ref<float,          3>    image_stack_ref;
  typedef  boost::const_multi_array_ref<float,          3>    image_stack_cref;
  typedef  image_stack::array_view<3>::type		image_stack_view;
  typedef  image_stack::array_view<2>::type		image_stack_frame;
  typedef  image_stack::array_view<1>::type		image_stack_line;
  typedef  boost::multi_array_types::index_range	range;
  typedef  boost::general_storage_order<3>		storage;

  std::ostream& operator<<(std::ostream&, const image_stack&);

  template <typename StorageT, typename DimT, typename ODimT>
  void adapt_extents_for_fftw_inplace(const StorageT& _storage, const DimT& _extent, ODimT& _value){

    std::fill(_value.begin(),_value.end(),0);

    std::vector<int> storage_order(_extent.size());
    for(size_t i = 0;i<_extent.size();++i)
      storage_order[i] = _storage.ordering(i);

    size_t lowest_storage_index = std::min_element(storage_order.begin(),storage_order.end()) - storage_order.begin() ;

    for(size_t i = 0;i<_extent.size();++i)
      _value[i] = (lowest_storage_index == i) ? 2*(_extent[i]/2 + 1) : _extent[i];  
  
  
  }

std::ostream& operator<<(std::ostream& _cout, const image_stack& _marray){

  if(image_stack::dimensionality!=3){
    _cout << "dim!=3\n";
    return _cout;
  }


  if(_marray.empty()){
    _cout << "size=0\n";
    return _cout;
  }
  
  int precision = _cout.precision();
  _cout << std::setprecision(4);
  const size_t* shapes = _marray.shape(); 
  
  _cout << std::setw(9) << "x = ";
  for(size_t x_index = 0;x_index<(shapes[0]);++x_index){
    _cout << std::setw(8) << x_index << " ";
  }
  _cout << "\n";
  _cout << std::setfill('-') << std::setw((shapes[0]+1)*9) << " " << std::setfill(' ')<< std::endl ;

  for(size_t z_index = 0;z_index<(shapes[2]);++z_index){
    _cout << "z["<< std::setw(5) << z_index << "] \n";
    for(size_t y_index = 0;y_index<(shapes[1]);++y_index){
      _cout << "y["<< std::setw(5) << y_index << "] ";

      for(size_t x_index = 0;x_index<(shapes[0]);++x_index){
        _cout << std::setw(8) << _marray[x_index][y_index][z_index] << " ";
      }

      _cout << "\n";
    }
    _cout << "\n";
  }

  _cout << std::setprecision(precision);
  return _cout;
}


}

#endif /* _IMAGE_STACK_UTILS_H_ */
