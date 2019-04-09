/*****************************************************************************
 **
 ** Copyright (C) 2019 Fabian Schweinfurth
 ** Contact: autoshift <at> derfabbi.de
 **
 ** This file is part of autoshift
 **
 ** autoshift is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** autoshift is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with autoshift.  If not, see <http://www.gnu.org/licenses/>.
 **
 *****************************************************************************/
#pragma once

#include <key.hpp>
#include <QUrl>
#include <QNetworkAccessManager>

#include <misc/fsettings.hpp>
#include <controlwindow.hpp>

/** @class CodeParser
 * @brief A parser for SHiFT codes
 *
 * This class queries and parses various sources for SHiFT codes
 */
// class ControlWindow;

class CodeParser
{
public:
  CodeParser():
    network_manager(static_cast<QNetworkAccessManager*>(FSETTINGS["nman"].value<void*>()))
  {}
  CodeParser(ControlWindow& cw, Game _g, Platform _p, const QIcon& _i = QIcon()):
    CodeParser(cw, {_g, Game::NONE}, {_p, Platform::NONE}, {_i})
  {}

  // CodeParser(ControlWindow& cw, Game _gs[], Platform _ps[], const QIcon& _i = QIcon()):
  CodeParser(ControlWindow& cw, std::initializer_list<Game> _gs,
             std::initializer_list<Platform> _ps, std::initializer_list<QIcon> _i):
    CodeParser()
  {
    // register this parser
    auto icon_it = _i.begin();

    for (Game _g: _gs) {
      if (_g == Game::NONE) continue;
      QIcon icn;
      if (icon_it != _i.end()) {
        icn = *icon_it;
        ++icon_it;
      }
      for (Platform _p: _ps) {
        if (_p == Platform::NONE) continue;
        cw.registerParser(_g, _p, this, icn);
      }
    }
  }

  /**
   * Parse keys and add them to the passed ShiftCollection.
   *
   * This function has spam-protection.
   *
   * @sa parse_keys(ShiftCollection&)
   *
   * @param coll output variable to contain the shift codes after parsing
   */
  void parseKeys(ShiftCollection& /* out */ coll)
  {

    parse_keys(coll);
  }

  /**
   * Parse keys and add them to the passed ShiftCollection.
   *
   * This function has no spam-protection.
   *
   * @sa parseKeys(ShiftCollection&)
   *
   * @param coll output variable to contain the shift codes after parsing
   */
  virtual void parse_keys(ShiftCollection& /* out */) = 0;

protected:
  QNetworkAccessManager* network_manager;
};

/** @class BL2nBLPSParser
 * BL2 and BLPS code Parser
 */
class BL2nBLPSParser: public CodeParser
{
public:
  BL2nBLPSParser(ControlWindow&, Game);
  void parse_keys(ShiftCollection&);
private:
  Game game;
  const QUrl& url;
  ShiftCollection collections[3];
};

class BL2Parser: public BL2nBLPSParser
{
public:
  BL2Parser(ControlWindow& cw):
    BL2nBLPSParser(cw, Game::BL2)
  {}
};

class BLPSParser: public BL2nBLPSParser
{
public:
  BLPSParser(ControlWindow& cw):
    BL2nBLPSParser(cw, Game::BLPS)
  {}
};