#include "ccartridgemodel.h"

#include "cnesicideproject.h"
#include "model/projectsearcher.h"

CCartridgeModel::CCartridgeModel()
{
}

QList<QUuid> CCartridgeModel::getPrgRomUuids() const
{
   if (m_pProject == NULL)
      return QList<QUuid>();

   return ProjectSearcher::findUuidsOfType<CPRGROMBank>(m_pProject);
}

QList<QUuid> CCartridgeModel::getChrRomUuids() const
{
   if (m_pProject == NULL)
      return QList<QUuid>();

   return ProjectSearcher::findUuidsOfType<CCHRROMBank>(m_pProject);
}

QList<QUuid> CCartridgeModel::getUuids() const
{
   QList<QUuid> items;
   items.append(ProjectSearcher::findUuidsOfType<CPRGROMBank>(m_pProject));
   items.append(ProjectSearcher::findUuidsOfType<CCHRROMBank>(m_pProject));
   return items;
}

QString CCartridgeModel::getName(const QUuid &uuid) const
{
   CPRGROMBank* prgBank = ProjectSearcher::findItemByUuid<CPRGROMBank>(m_pProject, uuid);
   if (prgBank != NULL)
      return prgBank->caption();

   CCHRROMBank* chrBank = ProjectSearcher::findItemByUuid<CCHRROMBank>(m_pProject, uuid);
   if (chrBank != NULL)
      return chrBank->caption();

   return QString();
}

CDesignerEditorBase *CCartridgeModel::createEditorWidget(const QUuid &) const
{
   return NULL;
}
