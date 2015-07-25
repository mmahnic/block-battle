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

#include "myai.h"
#include "dumps.h"

void MyAi::dropToColumn( int32_t nx )
{
   auto x = round()->pieceX;
   if ( nx < x )
      mAction.left( x - nx );
   else if ( nx > x )
      mAction.right( nx - x );
   mAction.drop();
}

void MyAi::makeSomeMoves()
{
   int32_t nr = mRand() % currentPiece()->shapes.size();
   if ( nr > 0 )
      mAction.turnRight( nr );

   int32_t nx = mRand() % settings()->fieldWidth;
   dropToColumn( nx );

   mAction.emit();
}
