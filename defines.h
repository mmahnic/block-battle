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
 * @created: 2015-07-18
 */

#pragma once

#define DBGERR( x ) std::cerr << " ** " << x

#if 1
#define DBGMSG( x ) std::cerr << " -- " << x
#else
#define DBGMSG( x )
#endif

#if 1
#define DBGTRACE( x ) std::cerr << " !! " << x << "\n" << std::flush
#else
#define DBGTRACE( x )
#endif

#define ITALL( c ) c.begin(), c.end()

