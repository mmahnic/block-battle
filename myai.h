/**
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
 */

#pragma once

#include "game.h"
#include <random>

#include "defines.h"

class MyAi: public Ai
{
   std::mt19937 mRand;
public:
   MyAi( ActionWriter& writer )
      : Ai( writer ), mRand(time(0))
   { }
   void dropToColumn( int32_t nx );
   void makeSomeMoves() override;
};
