// Copyright (c) 2011-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SUPCOIN_QT_SUPCOINADDRESSVALIDATOR_H
#define SUPCOIN_QT_SUPCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class SupcoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit SupcoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Supcoin address widget validator, checks for a valid supcoin address.
 */
class SupcoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit SupcoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // SUPCOIN_QT_SUPCOINADDRESSVALIDATOR_H
