#pragma once
#include <Rendering/KDNode.h>

class KDTree
  {
  public:
    KDTree();
    KDTree(KDTreeObjects&& i_objects);

    bool IntersectWithRay(
      IntersectionRecord& io_intersection, 
      const Ray& i_ray) const;

    std::size_t Size() const;
    void Clear();
    KDTreeObjects GetObjects() const;

    void AddObject(IRenderableSPtr i_object);

    void Update();

  private:
    KDNode m_root;
    KDTreeObjects m_objects;
  };

inline bool KDTree::IntersectWithRay(
  IntersectionRecord& io_intersection, 
  const Ray& i_ray) const
  {
  return m_root.IntersectWithRay(io_intersection, i_ray);
  }

inline std::size_t KDTree::Size() const
  {
  return m_objects.size();
  }

inline void KDTree::Clear()
  {
  m_objects.clear();
  m_root.Clear();
  }

inline KDTreeObjects KDTree::GetObjects() const
  {
  return m_objects;
  }

inline void KDTree::AddObject(IRenderableSPtr i_object)
  {
  m_objects.push_back(i_object);
  m_root.Clear();
  m_root.Build(m_objects);
  }

inline void KDTree::Update()
  {
  for (auto& object : m_objects)
    object->Update();
  m_root.Clear();
  m_root.Build(m_objects);
  }