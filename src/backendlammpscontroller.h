#ifndef BACKENDLAMMPSCONTROLLER_H
#define BACKENDLAMMPSCONTROLLER_H

#include <QBackendNode>
#include <QHash>

namespace atomify {

class BackendLAMMPSController : public Qt3DCore::QBackendNode
{
public:
    BackendLAMMPSController();
    void synchronize();
    // QBackendNode interface
protected:
    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e) override;

private:
    void initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change) override;
    QScopedPointer<class LAMMPSThread> m_thread;
    Qt3DCore::QNodeId m_spheresBufferId;
};

class LAMMPSControllerMapper : public Qt3DCore::QBackendNodeMapper
{
public:
    explicit LAMMPSControllerMapper(class LAMMPSAspect *aspect);
    Qt3DCore::QBackendNode *create(const Qt3DCore::QNodeCreatedChangeBasePtr &change) const override;
    Qt3DCore::QBackendNode *get(Qt3DCore::QNodeId id) const override;
    void destroy(Qt3DCore::QNodeId id) const override;
    QList<class BackendLAMMPSController*> controllers();

private:
    mutable QHash<Qt3DCore::QNodeId, class BackendLAMMPSController*> m_controllers;
};
}

#endif // BACKENDLAMMPSCONTROLLER_H