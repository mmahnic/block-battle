/**
 * Copyright 2015 Marko Mahnič
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * @author: Marko Mahnič
 * @created: 2015-07-11
 */

#pragma once

#include "game.h"
#include <iostream>

template<typename T>
struct DumpWrapper
{
   const T& v;
   DumpWrapper(const T& v_)
      : v(v_)
   {}
};

template<typename T>
DumpWrapper<T> Dump(const T& v_)
{
   return DumpWrapper<T>(v_);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const DumpWrapper<T>& obj)
{
   dump( obj.v, os );
}

template<typename T>
void dump( const std::vector<T>& items, std::ostream& out )
{
   out << "{";
   int32_t count = items.size();
   for ( const auto& i : items ) {
      dump( i, out );
      --count;
      if ( count > 0 )
         out << ", ";
   }
   out << "}";
}

inline
void dump( const Coord& c, std::ostream& out )
{
   out << "(" << c.r << ", " << c.c << ")";
}

inline
void dump( const Shape& s, std::ostream& out )
{
   out << "\nShape size: " << s.size() << ", coords: " << Dump( s.coords );
}

inline
void dump( const Piece& p, std::ostream& out )
{
   out << "Piece: " << p.id << "(" << p.shapes.size() << ")\n";
   for ( int r = 0; r < p.shapes[0].size(); ++r ) {
      out << "      ";
      for ( auto& s : p.shapes ) {
         for ( auto v : s.lines[r] )
            out << ( v ? "#" : "." );
         out << "  ";
      }
      out << "\n";
   }
   dump( p.shapes, out  );
   out << "\n";
}

inline
void dump( const Round& r, std::ostream& out )
{
   out << "id: " << r.id << "\n";
   out << "pieceX: " << r.pieceX << ", pieceY: " << r.pieceY << "\n";
   out << "thisPiece: " << r.thisPiece << ", nextPiece: " << r.nextPiece << "\n";
   out << "\n";
}

inline
void dump( const PlayerState& p, std::ostream& out )
{
   out << "name: " << p.name << "\n";
   out << "rowPoints: " << p.rowPoints << "\n";
   out << "combo: " << p.combo << "\n";
   out << "field rows: " << p.field.rows.size();
   if ( p.field.rows.size() > 0 )
      out << ", row[0]: " << p.field.rows[0].size();
   out << "\n\n";
}

inline
void dump( const Settings& sett, std::ostream& out )
{
   out << "timeBank: " << sett.timeBank << ", timePerMove: " << sett.timePerMove << "\n";
   out << "fieldWidth: " << sett.fieldWidth << ", fieldHeight: " << sett.fieldHeight << "\n";
   out << "playerNames: ";
   for ( auto n : sett.playerNames )
      out << n << " ";
   out << "\n";
   out << "myName: " << sett.myName << "\n";
   out << "pieces: " << sett.pieces.size() << "\n";
   for ( auto p : sett.pieces )
      dump( *p.second, out );
   out << "\n";
}

inline
void dump( const TheGame& g, std::ostream& out )
{
   out << "## Settings:\n" << Dump( *g.mpSettings );
   out << "## Round:\n" << Dump( *g.mpRound );
   out << "## Players:\n";
   for ( auto p : g.mPlayers )
      dump( *p, out );
   out << "My Player: " << g.mpMyPlayer->name << "\n\n";
}

template<typename T>
void dump( const T& item, std::ostream& out )
{
   out << item;
}

template<typename Iter>
void dump( Iter ib, int32_t count, std::ostream& out )
{
   out << "{";
   while ( count > 0 ) {
      dump( *ib, out );
      ++ib;
      --count;
      if ( count > 0 )
         out << ", ";
   }
   out << "}";
}

