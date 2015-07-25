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

#include "game.h"
#include "parsers.h"
#include "dumps.h"
#include "inputhandler.h"
#include "myai.h"

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <memory>
#include <random>
#include <algorithm>
#include <fstream>

#include "defines.h"

using namespace std;

class BlockBot
{
   std::shared_ptr<TheGame> mpGame;
   std::shared_ptr<Ai> mpAi;
   SettingsParser mSettParser;
   EntityUpdateParser mEntParser;
   ActionRequestParser mActionParser;
public:
   InputHandler mHandler;

public:
   BlockBot( std::shared_ptr<TheGame> pgame )
      : mpGame( pgame ), mSettParser( pgame->mpSettings ), mEntParser( pgame )
   {
      mSettParser.registerHandlers( mHandler );
      mEntParser.registerHandlers( mHandler );
      mActionParser.registerHandlers( mHandler );
   }

   void setAi( std::shared_ptr<Ai> pai )
   {
      mpAi = pai;
      if ( pai != nullptr )
         pai->setGame( mpGame );
      mActionParser.setAi( mpAi );
   }

   void run( istream& input )
   {
      string command, rest;
      bool starting = true;
      while ( input >> command ) {
         if ( starting ) {
            // we can parse special streams up to the first action; see sendFakeInput
            if ( command == "action" )
               starting = false;
            if ( command == "[[STREAMEND]]" )
               break;
         }
         getline( input, rest );
         std::stringstream ss( rest );
         if ( !mHandler.tryHandle( command, ss )) {
            DBGERR( "Unknown command: " << command << rest << "\n" );
         }
      }
   }
};

void sendFakeInput( BlockBot& bot )
{
   // We define pieces as a setting
   std::string pieces =
      "\nsettings piece I 4 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0;0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0"
      "\nsettings piece J 3 1,0,0,1,1,1,0,0,0;0,1,1,0,1,0,0,1,0;0,0,0,1,1,1,0,0,1;0,1,0,0,1,0,1,1,0"
      "\nsettings piece L 3 0,0,1,1,1,1,0,0,0;0,1,0,0,1,0,0,1,1;0,0,0,1,1,1,1,0,0;1,1,0,0,1,0,0,1,0"
      "\nsettings piece O 2 1,1,1,1"
      "\nsettings piece S 3 0,1,1,1,1,0,0,0,0;0,1,0,0,1,1,0,0,1"
      "\nsettings piece T 3 0,1,0,1,1,1,0,0,0;0,1,0,0,1,1,0,1,0;0,0,0,1,1,1,0,1,0;0,1,0,1,1,0,0,1,0"
      "\nsettings piece Z 3 1,1,0,0,1,1,0,0,0;0,0,1,0,1,1,0,1,0"
      "\n[[STREAMEND]]";
   std::stringstream ssf( pieces );
   bot.run( ssf );
}

#if defined(DEBUG_INTRFC)
class DebugParser
{
   std::shared_ptr<TheGame> mpGame;
public:
   DebugParser( std::shared_ptr<TheGame> pGame )
      : mpGame( pGame )
   { }
   void registerHandlers( InputHandler& parentHandler )
   {
      parentHandler.addHandler( "hello", [](istream&) { DBGMSG( "hi!\n" ); } );
      parentHandler.addHandler( "dump", [this](istream& input) {
            cerr << Dump( *mpGame );
            std::string rest;
            getline( input, rest );
         });
      auto ignore = [this](istream& input) {
         std::string rest;
         getline( input, rest );
      };
      parentHandler.addHandler( "#", ignore ); // comment lines in input
      parentHandler.addHandler( "Output", ignore );
      parentHandler.addHandler( "Round", ignore );
      parentHandler.addHandler( "quit", [this](istream& input) {
            cout << std::flush;
            cerr << std::flush;
            exit( 0 );
         });
   }
};
#endif

int main( int argc, char* argv[] )
{
   cout.sync_with_stdio( false );
   auto pGame = std::make_shared<TheGame>();
   BlockBot bot( pGame );
   ActionWriter writer( cout );

   auto pai = std::make_shared<MyAi>( writer );
   bot.setAi( pai );

   sendFakeInput( bot );

#if defined(DEBUG_INTRFC)
   DebugParser debug( pGame );
   debug.registerHandlers( bot.mHandler );
   if ( argc > 1 ) {
      std::ifstream fin( argv[1] );
      bot.run( fin );
   }
   else
      bot.run( cin );
#else
   bot.run( cin );
#endif

   return 0;
}
