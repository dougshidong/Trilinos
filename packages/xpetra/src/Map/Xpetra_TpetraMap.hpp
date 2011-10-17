#ifndef XPETRA_TPETRAMAP_HPP
#define XPETRA_TPETRAMAP_HPP

/* this file is automatically generated - do not edit (see script/tpetra.py) */

#include "Xpetra_TpetraConfigDefs.hpp"

#include <Tpetra_Map.hpp>

#include "Xpetra_Map.hpp"
#include "Xpetra_Utils.hpp"

#include "Xpetra_Exceptions.hpp"

namespace Xpetra {

  // TODO: move that elsewhere
  template <class LocalOrdinal, class GlobalOrdinal, class Node>
  const Tpetra::Map<LocalOrdinal,GlobalOrdinal,Node> & toTpetra(const Xpetra::Map<LocalOrdinal,GlobalOrdinal,Node> &);

  template <class LocalOrdinal, class GlobalOrdinal, class Node>
  const RCP< const Tpetra::Map< LocalOrdinal, GlobalOrdinal, Node > > & toTpetra(const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &);

  template <class LocalOrdinal, class GlobalOrdinal, class Node>
  const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > toXpetra(const RCP< const Tpetra::Map< LocalOrdinal, GlobalOrdinal, Node > > &);
  //

  template <class LocalOrdinal, class GlobalOrdinal = LocalOrdinal, class Node = Kokkos::DefaultNode::DefaultNodeType>
  class TpetraMap
    : public Map<LocalOrdinal,GlobalOrdinal,Node> {

  public:

    //! @name Constructor/Destructor Methods
    //@{

    //! Map constructor with Tpetra-defined contiguous uniform distribution. The elements are distributed among nodes so that the subsets of global elements are non-overlapping and contiguous and as evenly distributed across the nodes as possible.
    TpetraMap(global_size_t numGlobalElements, GlobalOrdinal indexBase, const Teuchos::RCP< const Teuchos::Comm< int > > &comm, LocalGlobal lg=GloballyDistributed, const Teuchos::RCP< Node > &node=Kokkos::DefaultNode::getDefaultNode())
      : map_(Teuchos::rcp(new Tpetra::Map< LocalOrdinal, GlobalOrdinal, Node >(numGlobalElements, indexBase, comm, toTpetra(lg), node))) { }

    //! Map constructor with a user-defined contiguous distribution. The elements are distributed among the nodes so that the subsets of global elements are non-overlapping and contiguous.
    TpetraMap(global_size_t numGlobalElements, size_t numLocalElements, GlobalOrdinal indexBase, const Teuchos::RCP< const Teuchos::Comm< int > > &comm, const Teuchos::RCP< Node > &node=Kokkos::DefaultNode::getDefaultNode())
      : map_(Teuchos::rcp(new Tpetra::Map< LocalOrdinal, GlobalOrdinal, Node >(numGlobalElements, numLocalElements, indexBase, comm, node))) { }

    //! Map constructor with user-defined non-contiguous (arbitrary) distribution.
    TpetraMap(global_size_t numGlobalElements, const Teuchos::ArrayView< const GlobalOrdinal > &elementList, GlobalOrdinal indexBase, const Teuchos::RCP< const Teuchos::Comm< int > > &comm, const Teuchos::RCP< Node > &node=Kokkos::DefaultNode::getDefaultNode())
      : map_(Teuchos::rcp(new Tpetra::Map< LocalOrdinal, GlobalOrdinal, Node >(numGlobalElements, elementList, indexBase, comm, node))) { }

    //! Map destructor.
    ~TpetraMap() { }

    //@}

    //! @name Map Attribute Methods
    //@{

    //! Returns the number of elements in this Map.
    global_size_t getGlobalNumElements() const { return map_->getGlobalNumElements(); }

    //! Returns the number of elements belonging to the calling node.
    size_t getNodeNumElements() const { return map_->getNodeNumElements(); }

    //! Returns the index base for this Map.
    GlobalOrdinal getIndexBase() const { return map_->getIndexBase(); }

    //! Returns minimum local index.
    LocalOrdinal getMinLocalIndex() const { return map_->getMinLocalIndex(); }

    //! Returns maximum local index.
    LocalOrdinal getMaxLocalIndex() const { return map_->getMaxLocalIndex(); }

    //! Returns minimum global index owned by this node.
    GlobalOrdinal getMinGlobalIndex() const { return map_->getMinGlobalIndex(); }

    //! Returns maximum global index owned by this node.
    GlobalOrdinal getMaxGlobalIndex() const { return map_->getMaxGlobalIndex(); }

    //! Return the minimum global index over all nodes.
    GlobalOrdinal getMinAllGlobalIndex() const { return map_->getMinAllGlobalIndex(); }

    //! Return the maximum global index over all nodes.
    GlobalOrdinal getMaxAllGlobalIndex() const { return map_->getMaxAllGlobalIndex(); }

    //! Return the local index for a given global index.
    LocalOrdinal getLocalElement(GlobalOrdinal globalIndex) const { return map_->getLocalElement(globalIndex); }

    //! Return the global index for a given local index.
    GlobalOrdinal getGlobalElement(LocalOrdinal localIndex) const { return map_->getGlobalElement(localIndex); }

    //! Returns the node IDs and corresponding local indices for a given list of global indices.
    LookupStatus getRemoteIndexList(const Teuchos::ArrayView< const GlobalOrdinal > &GIDList, const Teuchos::ArrayView< int > &nodeIDList, const Teuchos::ArrayView< LocalOrdinal > &LIDList) const { return toXpetra(map_->getRemoteIndexList(GIDList, nodeIDList, LIDList)); }

    //! Returns the node IDs for a given list of global indices.
    LookupStatus getRemoteIndexList(const Teuchos::ArrayView< const GlobalOrdinal > &GIDList, const Teuchos::ArrayView< int > &nodeIDList) const { return toXpetra(map_->getRemoteIndexList(GIDList, nodeIDList)); }

    //! Return a list of the global indices owned by this node.
    Teuchos::ArrayView< const GlobalOrdinal > getNodeElementList() const { return map_->getNodeElementList(); }

    //! Returns true if the local index is valid for this Map on this node; returns false if it isn't.
    bool isNodeLocalElement(LocalOrdinal localIndex) const { return map_->isNodeLocalElement(localIndex); }

    //! Returns true if the global index is found in this Map on this node; returns false if it isn't.
    bool isNodeGlobalElement(GlobalOrdinal globalIndex) const { return map_->isNodeGlobalElement(globalIndex); }

    //! Returns true if this Map is distributed contiguously; returns false otherwise.
    bool isContiguous() const { return map_->isContiguous(); }

    //! Returns true if this Map is distributed across more than one node; returns false otherwise.
    bool isDistributed() const { return map_->isDistributed(); }

    //@}

    //! @name Boolean Tests
    //@{

    //! Returns true if map is compatible with this Map.
    bool isCompatible(const Map< LocalOrdinal, GlobalOrdinal, Node > &map) const { return map_->isCompatible(toTpetra(map)); }

    //! Returns true if map is identical to this Map.
    bool isSameAs(const Map< LocalOrdinal, GlobalOrdinal, Node > &map) const { return map_->isSameAs(toTpetra(map)); }

    //@}

    //! @name 
    //@{

    //! Get the Comm object for this Map.
    const Teuchos::RCP< const Teuchos::Comm< int > >  getComm() const { return map_->getComm(); }

    //! Get the Node object for this Map.
    const Teuchos::RCP< Node >  getNode() const { return map_->getNode(); }

    //@}

    //! @name 
    //@{

    //! Return a simple one-line description of this object.
    std::string description() const { return map_->description(); }

    //! Print the object with some verbosity level to a FancyOStream object.
    void describe(Teuchos::FancyOStream &out, const Teuchos::EVerbosityLevel verbLevel=Teuchos::Describable::verbLevel_default) const { map_->describe(out, verbLevel); }

    //@}

    //! @name Xpetra specific
    //@{

    //! TpetraMap constructor to wrap a Tpetra::Map object
    TpetraMap(const Teuchos::RCP<const Tpetra::Map<LocalOrdinal, GlobalOrdinal, Node > > &map) 
      : map_(map) { }

    //! Get the library used by this object (Tpetra or Epetra?)
    UnderlyingLib lib() const { return Xpetra::UseTpetra; }

    //! Get the underlying Tpetra map
    const RCP< const Tpetra::Map< LocalOrdinal, GlobalOrdinal, Node > > & getTpetra_Map() const { return map_; }

    //@}
   
  private:

    const RCP< const Tpetra::Map< LocalOrdinal, GlobalOrdinal, Node > > map_;

  }; // TpetraMap class

  // TODO: move that elsewhere
  template <class LocalOrdinal, class GlobalOrdinal, class Node>
  const Tpetra::Map<LocalOrdinal,GlobalOrdinal,Node> & toTpetra(const Map<LocalOrdinal,GlobalOrdinal,Node> &map) {
    // TODO: throw exception
    const TpetraMap<LocalOrdinal,GlobalOrdinal,Node> & tpetraMap = dynamic_cast<const TpetraMap<LocalOrdinal,GlobalOrdinal,Node> &>(map);
    return *tpetraMap.getTpetra_Map();
  }

  template <class LocalOrdinal, class GlobalOrdinal, class Node>
  const RCP< const Tpetra::Map< LocalOrdinal, GlobalOrdinal, Node > > & toTpetra(const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &map) {
    typedef TpetraMap<LocalOrdinal, GlobalOrdinal, Node> TpetraMapClass;
    XPETRA_RCP_DYNAMIC_CAST(const TpetraMapClass, map, tpetraMap, "toTpetra");
    return tpetraMap->getTpetra_Map();
  }

  template <class LocalOrdinal, class GlobalOrdinal, class Node>
  const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > toXpetra(const RCP< const Tpetra::Map< LocalOrdinal, GlobalOrdinal, Node > > &map) {
    return rcp( new TpetraMap<LocalOrdinal, GlobalOrdinal, Node>(map));
  }
  //

  // TODO: removed (but currently used in unit test)
  namespace useTpetra {

    //! Non-member function to create a locally replicated Map with a specified node.
    template <class LocalOrdinal, class GlobalOrdinal, class Node>
    Teuchos::RCP< const TpetraMap<LocalOrdinal,GlobalOrdinal,Node> >
    createLocalMapWithNode(size_t numElements, const Teuchos::RCP< const Teuchos::Comm< int > > &comm, const Teuchos::RCP< Node > &node) { 
      return rcp(new TpetraMap<LocalOrdinal,GlobalOrdinal,Node>(Tpetra::createLocalMapWithNode<LocalOrdinal,GlobalOrdinal,Node>(numElements, comm, node)));
    }

    //! Non-member function to create a (potentially) non-uniform, contiguous Map with the default node.
    template <class LocalOrdinal, class GlobalOrdinal>
    Teuchos::RCP< const TpetraMap<LocalOrdinal,GlobalOrdinal,Kokkos::DefaultNode::DefaultNodeType> >
    createContigMap(global_size_t numElements, size_t localNumElements, const Teuchos::RCP< const Teuchos::Comm< int > > &comm) { 
      return rcp(new TpetraMap<LocalOrdinal,GlobalOrdinal>(Tpetra::createContigMap<LocalOrdinal,GlobalOrdinal>(numElements, localNumElements, comm)));
    }

    //! Non-member function to create a (potentially) non-uniform, contiguous Map with a user-specified node.
    template <class LocalOrdinal, class GlobalOrdinal, class Node>
    Teuchos::RCP< const TpetraMap<LocalOrdinal,GlobalOrdinal,Node> >
    createContigMapWithNode(global_size_t numElements, size_t localNumElements, 
                            const Teuchos::RCP< const Teuchos::Comm< int > > &comm, const Teuchos::RCP< Node > &node) { 
      return rcp(new TpetraMap<LocalOrdinal,GlobalOrdinal,Node>(Tpetra::createContigMapWithNode<LocalOrdinal,GlobalOrdinal,Node>(numElements, localNumElements, comm, node)));
    }
  } // useTpetra namespace

} // Xpetra namespace

// TODO: remove?
//!  Returns true if \c map is identical to this map. Implemented in TpetraMap::isSameAs().
template <class LocalOrdinal, class GlobalOrdinal, class Node>
bool operator== (const Xpetra::TpetraMap<LocalOrdinal,GlobalOrdinal,Node> &map1, const Xpetra::TpetraMap<LocalOrdinal,GlobalOrdinal,Node> &map2) { 
  return map1.isSameAs(map2);
}

//! Returns true if \c map is not identical to this map. Implemented in TpetraMap::isSameAs().
template <class LocalOrdinal, class GlobalOrdinal, class Node>
bool operator!= (const Xpetra::TpetraMap<LocalOrdinal,GlobalOrdinal,Node> &map1, const Xpetra::TpetraMap<LocalOrdinal,GlobalOrdinal,Node> &map2) { 
  return !map1.isSameAs(map2);
}

#define XPETRA_TPETRAMAP_SHORT
#endif // XPETRA_TPETRAMAP_HPP
