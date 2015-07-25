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

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>

struct Coord
{
   int32_t r;
   int32_t c;
   Coord( int32_t r_, int32_t c_ )
      : r( r_ ), c( c_ )
   { }
};

struct Shape
{
   using cell_t = int32_t;
   using coord_t = Coord;
   using line_t = std::vector<cell_t>;
   std::vector<line_t> lines;
   std::vector<coord_t> coords;
   Shape( std::vector<line_t> lines_ )
      : lines( lines_ )
   {
      for ( int r = 0; r < lines.size(); ++r )
         for ( int c = 0; c < lines[r].size(); ++c )
            if ( lines[r][c] )
               coords.push_back( Coord( r, c ));
   }
   int32_t size() const {
      return lines.size();
   }
};

struct Piece
{
   char id = 0;
   int32_t size = 0;
   std::vector<Shape> shapes;
   Piece( char pieceId, int32_t edgeSize )
      : id( pieceId ), size( edgeSize )
   { }
};

struct Settings
{
   int32_t timeBank = 0;
   int32_t timePerMove = 0;
   int32_t fieldHeight = 0;
   int32_t fieldWidth = 0;
   std::vector<std::string> playerNames;
   std::string myName;
   std::unordered_map<char, std::shared_ptr<Piece>> pieces;
};

struct Round
{
   int32_t id = 0;
   int32_t pieceX = 0;
   int32_t pieceY = 0;
   char thisPiece = 0;
   char nextPiece = 0;
};

struct Field
{
   using cell_t = int32_t;
   using row_t = std::vector<cell_t>;
   std::vector<row_t> rows;
};

struct PlayerState
{
   std::string name;
   int32_t rowPoints = 0;
   int32_t combo = 0;
   Field field;
   PlayerState( std::string playerName )
      : name( playerName )
   { }

};

struct TheGame
{
   std::shared_ptr<Settings> mpSettings;
   std::shared_ptr<Round> mpRound;
   std::vector<std::shared_ptr<PlayerState>> mPlayers;
   std::shared_ptr<PlayerState> mpMyPlayer;

   TheGame()
   {
      mpSettings = std::make_shared<Settings>();
      mpRound = std::make_shared<Round>();
   }

   void initPlayers()
   {
      if ( mPlayers.size() > 0 )
         return;

      for ( auto name : mpSettings->playerNames ) {
         auto player = std::make_shared<PlayerState>( name );
         mPlayers.push_back( player );
         if ( name == mpSettings->myName )
            mpMyPlayer = player;
      }
   }
};

struct ActionWriter
{
protected:
   std::ostream& mOutput;
   bool first = true;

   void append( const std::string& command, int32_t nr )
   {
      while ( nr-- > 0 ) {
         if ( first )
            first = false;
         else
            mOutput << ",";
         mOutput << command;
      }
   };

public:
   ActionWriter( std::ostream& output )
      : mOutput( output )
   { }

   void emit()
   {
      mOutput << "\n";
      first = true;
   }

   void turnRight( int32_t nr=1 )
   {
      append( "turnright", nr );
   }

   void turnLeft( int32_t nr=1 )
   {
      append( "turnleft", nr );
   }

   void right( int32_t nr=1 )
   {
      append( "right", nr );
   }

   void left( int32_t nr=1 )
   {
      append( "left", nr );
   }

   void down( int32_t nr=1 )
   {
      append( "down", nr );
   }

   void drop()
   {
      append( "drop", 1 );
   }

};

class Ai
{
protected:
   std::shared_ptr<TheGame> mpGame;
   ActionWriter mAction;
   int32_t mTimeLeft;

public:
   Ai( ActionWriter& writer )
      : mAction( writer )
   { }

   void setGame( std::shared_ptr<TheGame> pgame )
   {
      mpGame = pgame;
   }

   void setTimeLeft( int32_t timeleft )
   {
      mTimeLeft = timeleft;
   }

   std::shared_ptr<Settings> settings()
   {
      // if ( !mpGame ) return nullptr;
      return mpGame->mpSettings;
   }

   std::shared_ptr<Round> round()
   {
      // if ( !mpGame ) return nullptr;
      return mpGame->mpRound;
   }

   std::shared_ptr<PlayerState> player()
   {
      // if ( !mpGame ) return nullptr;
      return mpGame->mpMyPlayer;
   }

   std::shared_ptr<PlayerState> opponent()
   {
      // if ( !mpGame ) return nullptr;
      // TODO: opponent could also be stored in TheGame
      for ( auto p : mpGame->mPlayers )
         if ( p != mpGame->mpMyPlayer )
            return p;
      return nullptr;
   }

   std::shared_ptr<Piece> currentPiece()
   {
      // if ( !mpGame || !mpGame->mpSettings || !mpGame->mpRound ) return nullptr;
      return mpGame->mpSettings->pieces[mpGame->mpRound->thisPiece];
   }

   std::shared_ptr<Piece> nextPiece()
   {
      // if ( !mpGame || !mpGame->mpSettings || !mpGame->mpRound ) return nullptr;
      return mpGame->mpSettings->pieces[mpGame->mpRound->nextPiece];
   }

   virtual void makeSomeMoves() = 0;
};

