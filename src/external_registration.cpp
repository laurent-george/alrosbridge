/*
 * Copyright 2015 Aldebaran
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
*/


#include <qi/applicationsession.hpp>
#include <qi/anymodule.hpp>
#include <alrosbridge/alrosbridge.hpp>

int main(int argc, char** argv)
{
  qi::ApplicationSession app(argc, argv);
  app.start();
  //app.session()->loadService( "alros.BridgeService" );
  //app.session()->registerService("BridgeService",
  // qi::import("alros").call<qi::AnyObject>("BridgeService", app.session()));
  boost::shared_ptr<alros::Bridge> bs = qi::import("alros").call<qi::Object<alros::Bridge> >("ALRosBridge", app.session()).asSharedPtr();
  app.session()->registerService("ALRosBridge", bs);


  if ( argc > 1 )
  {
    std::cout << BOLDYELLOW << "using ip address: "
              << BOLDCYAN << argv[1] << RESETCOLOR << std::endl;
    bs->setMasterURI( "http://"+std::string(argv[1])+":11311");
  }
  else
  {
    std::cout << BOLDRED << "No ip address given. Run qicli call to set the master uri" << RESETCOLOR << std::endl;
  }

  //! @note Must call ow._stopService when the application stops to do the clean-up
  app.atStop(boost::function<void()>(
                 boost::bind(&alros::Bridge::stopService, boost::ref(bs))));

  app.run();
  app.session()->close();
}
