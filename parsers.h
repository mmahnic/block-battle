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

#include "inputhandler.h"
#include "game.h"

#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <unordered_map>

#include "defines.h"

class SettingsParser
{
   InputHandler mHandler;
   std::shared_ptr<Settings> mpSettings;

public:
   SettingsParser( std::shared_ptr<Settings> psettings )
      : mpSettings( psettings )
   { }

   void registerHandlers( InputHandler& parentHandler )
   {
      parentHandler.addHandler( "settings", [this]( std::istream& input )
         {
            std::string param;
            input >> param;
            if ( !mHandler.tryHandle( param, input )) {
               std::string rest;
               std::getline( input, rest );
               DBGERR( "Unknown setting: " << param << rest << "\n" );
            }
         });

      mHandler.addHandler( "time_bank", [this]( std::istream& input )
         { input >> mpSettings->timeBank; });
      mHandler.addHandler( "timebank", [this]( std::istream& input )
         { input >> mpSettings->timeBank; });
      mHandler.addHandler( "time_per_move", [this]( std::istream& input )
         { input >> mpSettings->timePerMove; });
      mHandler.addHandler( "field_height", [this]( std::istream& input )
         { input >> mpSettings->fieldHeight; });
      mHandler.addHandler( "field_width", [this]( std::istream& input )
         { input >> mpSettings->fieldWidth; });
      mHandler.addHandler( "your_bot", [this]( std::istream& input )
         { input >> mpSettings->myName; });
      mHandler.addHandler( "player_names", [this]( std::istream& input )
         {
            mpSettings->playerNames.clear();
            std::string names, item;
            input >> names;
            std::stringstream ss(names);
            while (std::getline(ss, item, ','))
               mpSettings->playerNames.push_back(item);
            // for ( auto n : mpSettings->playerNames ) DBGMSG( n << "\n" );
         });
      mHandler.addHandler( "piece", [this]( std::istream& input )
         {
            char id;
            int size;
            std::string shapes, face, cell;
            input >> id;
            input >> size;
            input >> shapes;
            std::stringstream ss( shapes );
            auto piece = std::make_shared<Piece>( id, size );
            while( std::getline( ss, face, ';' )) {
               Shape::line_t line;
               std::vector<Shape::line_t> lines;
               std::stringstream sl( face );
               while( std::getline( sl, cell, ',' )) {
                  line.push_back( atoi( cell.c_str() ));
                  if ( line.size() == size ) {
                     lines.push_back( line );
                     line.clear();
                  }
               }
               if ( lines.size() != size )
                  DBGERR( "Not enough data for shape " << id << "(" << size << ") : " << face << "\n" );
               else
                  piece->shapes.push_back( Shape( lines ) );
            }
            if ( piece->shapes.size() > 0 )
               mpSettings->pieces[piece->id] = piece;
         });
   }
};

class RoundParser
{
   InputHandler mHandler;
   std::shared_ptr<Round> mpRound;
public:
   RoundParser( std::shared_ptr<Round> pround )
      : mpRound( pround )
   { }

   void registerHandlers( InputHandler& parentHandler )
   {
      parentHandler.addHandler( "game", [this]( std::istream& input )
         {
            std::string param;
            input >> param;
            if ( !mHandler.tryHandle( param, input )) {
               std::string rest;
               std::getline( input, rest );
               DBGERR( "Unknown game setting: " << param << rest << "\n" );
            }
         });

      mHandler.addHandler( "round", [this]( std::istream& input )
         { input >> mpRound->id; });
      mHandler.addHandler( "next_piece_type", [this]( std::istream& input )
         { input >> mpRound->nextPiece; });
      mHandler.addHandler( "this_piece_type", [this]( std::istream& input )
         { input >> mpRound->thisPiece; });
      mHandler.addHandler( "this_piece_position",
         [this]( std::istream& input )
         {
            std::string coords, item;
            input >> coords;
            std::stringstream ss(coords);
            int i = 0;
            while (std::getline(ss, item, ',')) {
               if ( i == 0 )
                  mpRound->pieceX = atoi( item.c_str() );
               else if ( i == 1 )
                  mpRound->pieceY = atoi( item.c_str() );
               else
                  break;
               ++i;
            }
         });
   }
};

class PlayerStateParser
{
   InputHandler mHandler;
   std::shared_ptr<PlayerState> mpState;
public:
   PlayerStateParser( std::shared_ptr<PlayerState> pstate )
      : mpState( pstate )
   { }

   void registerHandlers( std::string name, InputHandler& parentHandler )
   {
      parentHandler.addHandler( name, [this, name]( std::istream& input )
         {
            std::string param;
            input >> param;
            if ( !mHandler.tryHandle( param, input )) {
               std::string rest;
               std::getline( input, rest );
               DBGERR( "Unknown " << name << " setting: " << param << rest << "\n" );
            }
         });

      mHandler.addHandler( "row_points", [this]( std::istream& input )
         { input >> mpState->rowPoints; });
      mHandler.addHandler( "combo", [this]( std::istream& input )
         { input >> mpState->combo; });
      mHandler.addHandler( "field",
         [this]( std::istream& input )
         {
            std::string field;
            input >> field;
            std::stringstream ss(field);
            parseField( ss );
         });
   }

private:
   void parseField( std::istream& input )
   {
      mpState->field.rows.clear();
      std::string line, cell;
      while (std::getline(input, line, ';')) {
         Field::row_t row;
         std::stringstream ss(line);
         while (std::getline(ss, cell, ',')) {
            auto v = atoi( cell.c_str() );
            row.push_back( v > 1 ? v : 0 );
         }
         mpState->field.rows.push_back(row);
      }
   }
};

class EntityUpdateParser
{
   std::unordered_map<std::string, std::shared_ptr<PlayerStateParser>> mPlayerParsers;
   RoundParser mRoundParser;
   std::shared_ptr<TheGame> mpGame;

   InputHandler mHandler;
public:
   EntityUpdateParser( std::shared_ptr<TheGame> pGame )
      : mpGame( pGame ), mRoundParser( pGame->mpRound )
   { }

   void registerHandlers( InputHandler& parentHandler )
   {
      parentHandler.addHandler( "update",
            [this](std::istream& input) { updateEntity( input ); });

      mRoundParser.registerHandlers( mHandler );
   }

private:
   void updateEntity( std::istream& input )
   {
      if ( mPlayerParsers.size() == 0 )
         initPlayers();

      std::string entity;
      input >> entity;
      if ( !mHandler.tryHandle( entity, input )) {
         std::string rest;
         std::getline( input, rest );
         DBGERR( "Unknown entity setting: " << entity << rest << "\n" );
      }
   }

   void initPlayers()
   {
      mpGame->initPlayers();
      if ( mpGame->mPlayers.size() == 0 )
         DBGERR( "The game has no players.\n" );
      mPlayerParsers.clear();
      for ( auto pplayer : mpGame->mPlayers ) {
         auto parser = std::make_shared<PlayerStateParser>( pplayer );
         mPlayerParsers[pplayer->name] = parser;
         parser->registerHandlers( pplayer->name, mHandler );
      }
   }
};

class ActionRequestParser
{
   std::shared_ptr<Ai> mpAi;
public:
   ActionRequestParser()
   { }

   void setAi( std::shared_ptr<Ai> pai )
   {
      mpAi = pai;
   }

   void registerHandlers( InputHandler& parentHandler )
   {
      parentHandler.addHandler( "action", [this](std::istream& input) {
            int32_t timeleft;
            std::string what;
            input >> what;
            if ( what != "moves" ) {
               std::string rest;
               std::getline( input, rest );
               DBGERR( "Unknown action: " << what << rest << "\n" );
               return;
            }
            input >> timeleft;
            if ( mpAi != nullptr ) {
               mpAi->setTimeLeft( timeleft );
               mpAi->makeSomeMoves();
            }
         });
   }
};
