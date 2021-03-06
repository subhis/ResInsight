/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016 Statoil ASA
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

#include "RimPlotAxisPropertiesInterface.h"

#include "cafPdmObject.h"
#include "cafPdmField.h"
#include "cafPdmChildArrayField.h"
#include "cafAppEnum.h"

#include <QString>
#include <QDateTime>

//==================================================================================================
///  
///  
//==================================================================================================
class RimSummaryTimeAxisProperties : public caf::PdmObject, public RimPlotAxisPropertiesInterface
{
    CAF_PDM_HEADER_INIT;

public:
    enum TimeModeType
    {
        DATE,
        TIME_FROM_SIMULATION_START
    };

    enum TimeUnitType
    {
        SECONDS, 
        MINUTES, 
        HOURS, 
        DAYS, 
        YEARS
    };

public:
    RimSummaryTimeAxisProperties();

    caf::PdmField<QString>      title;
    caf::PdmField<bool>         showTitle;


    AxisTitlePositionType titlePosition() const override;
    int                   titleFontSize() const override;
    void                  setTitleFontSize(int fontSize) override;
    int                   valuesFontSize() const override;
    void                  setValuesFontSize(int fontSize) override;
    TimeModeType          timeMode() const;
    void                  setTimeMode(TimeModeType val);
    double                fromTimeTToDisplayUnitScale();
    double                fromDaysToDisplayUnitScale();

    double visibleRangeMin() const;
    double visibleRangeMax() const;

    void setVisibleRangeMin(double value);
    void setVisibleRangeMax(double value);

    bool isAutoZoom() const;
    void setAutoZoom(bool enableAutoZoom);

    bool isActive() const;

protected:
    void                            fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue) override;
    QList<caf::PdmOptionItemInfo>   calculateValueOptions(const caf::PdmFieldHandle* fieldNeedingOptions, bool * useOptionsOnly) override;
    caf::PdmFieldHandle*            objectToggleField() override;
    void                            defineUiOrdering(QString uiConfigName, caf::PdmUiOrdering& uiOrdering) override;

    double                                  fromDateToDisplayTime(const QDateTime& displayTime);
    QDateTime                               fromDisplayTimeToDate(double displayTime);
    void                                    updateTimeVisibleRange();
    void                                    updateDateVisibleRange();

private:
    caf::PdmField< caf::AppEnum< TimeModeType > > m_timeMode;
    caf::PdmField< caf::AppEnum< TimeUnitType > > m_timeUnit;

    caf::PdmField<bool>                     m_isActive;
    caf::PdmField<QDateTime>                m_visibleDateRangeMin;
    caf::PdmField<QDateTime>                m_visibleDateRangeMax;
    caf::PdmField<double>                   m_visibleTimeRangeMin;
    caf::PdmField<double>                   m_visibleTimeRangeMax;
    caf::PdmField<bool>                     m_isAutoZoom;

    caf::PdmField<int>                                 m_titleFontSize;
    caf::PdmField<caf::AppEnum<AxisTitlePositionType>> m_titlePositionEnum;
    caf::PdmField<int>                                 m_valuesFontSize;

};
