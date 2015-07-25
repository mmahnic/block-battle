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

#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>

class InputHandler
{
public:
   using handler_t = std::function<void( std::istream& )>;

private:
   std::unordered_map<std::string, handler_t> mHandlers;

public:
   void addHandler( std::string command, handler_t fn )
   {
      if ( fn == nullptr )
         mHandlers.erase( command );
      else
         mHandlers[command] = fn;
   }

   bool tryHandle( const std::string& command, std::istream& input )
   {
      auto it = mHandlers.find( command );
      if ( it == mHandlers.end() )
         return false;

      it->second( input );
      return true;
   }
};
