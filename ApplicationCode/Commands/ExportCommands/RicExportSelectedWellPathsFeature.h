/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
// 
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QFile>

#include "cafCmdFeature.h"

class RimWellPath;
class RicExportWellPathsUi;

//==================================================================================================
/// 
//==================================================================================================
typedef std::shared_ptr<QFile> QFilePtr;

//==================================================================================================
/// 
//==================================================================================================
class RicExportSelectedWellPathsFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

    static void exportWellPath(const RimWellPath* wellPath, double mdStepSize, const QString& folder);
    static QFilePtr openFileForExport(const QString& folderName, const QString& fileName);
    static void handleAction(const std::vector<RimWellPath*>& wellPaths);

protected:
    // Overrides
    virtual bool isCommandEnabled();
    virtual void onActionTriggered( bool isChecked );
    virtual void setupActionLook(QAction* actionToSetup);

private:
    static RicExportWellPathsUi* openDialog();
};