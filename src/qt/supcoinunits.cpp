// Copyright (c) 2011-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "supcoinunits.h"

#include "primitives/transaction.h"

#include <QStringList>

SupcoinUnits::SupcoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<SupcoinUnits::Unit> SupcoinUnits::availableUnits()
{
    QList<SupcoinUnits::Unit> unitlist;
    unitlist.append(SUP);
    unitlist.append(mSUP);
    unitlist.append(uSUP);
    return unitlist;
}

bool SupcoinUnits::valid(int unit)
{
    switch(unit)
    {
    case SUP:
    case mSUP:
    case uSUP:
        return true;
    default:
        return false;
    }
}

QString SupcoinUnits::id(int unit)
{
    switch(unit)
    {
    case SUP: return QString("btc");
    case mSUP: return QString("mbtc");
    case uSUP: return QString("ubtc");
    default: return QString("???");
    }
}

QString SupcoinUnits::name(int unit)
{
    switch(unit)
    {
    case SUP: return QString("SUP");
    case mSUP: return QString("mSUP");
    case uSUP: return QString::fromUtf8("μSUP");
    default: return QString("???");
    }
}

QString SupcoinUnits::description(int unit)
{
    switch(unit)
    {
    case SUP: return QString("Supcoins");
    case mSUP: return QString("Milli-Supcoins (1 / 1" THIN_SP_UTF8 "000)");
    case uSUP: return QString("Micro-Supcoins (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
    default: return QString("???");
    }
}

qint64 SupcoinUnits::factor(int unit)
{
    switch(unit)
    {
    case SUP:  return 100000000;
    case mSUP: return 100000;
    case uSUP: return 100;
    default:   return 100000000;
    }
}

int SupcoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case SUP: return 8;
    case mSUP: return 5;
    case uSUP: return 2;
    default: return 0;
    }
}

QString SupcoinUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');
    return quotient_str + QString(".") + remainder_str;
}


// TODO: Review all remaining calls to SupcoinUnits::formatWithUnit to
// TODO: determine whether the output is used in a plain text context
// TODO: or an HTML context (and replace with
// TODO: BtcoinUnits::formatHtmlWithUnit in the latter case). Hopefully
// TODO: there aren't instances where the result could be used in
// TODO: either context.

// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString SupcoinUnits::formatWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    return format(unit, amount, plussign, separators) + QString(" ") + name(unit);
}

QString SupcoinUnits::formatHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}


bool SupcoinUnits::parse(int unit, const QString &value, CAmount *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
    QStringList parts = removeSpaces(value).split(".");

    if(parts.size() > 2)
    {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if(parts.size() > 1)
    {
        decimals = parts[1];
    }
    if(decimals.size() > num_decimals)
    {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if(str.size() > 18)
    {
        return false; // Longer numbers will exceed 63 bits
    }
    CAmount retvalue(str.toLongLong(&ok));
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

QString SupcoinUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (SupcoinUnits::valid(unit))
    {
        amountTitle += " ("+SupcoinUnits::name(unit) + ")";
    }
    return amountTitle;
}

int SupcoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant SupcoinUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}

CAmount SupcoinUnits::maxMoney()
{
    return MAX_MONEY;
}
