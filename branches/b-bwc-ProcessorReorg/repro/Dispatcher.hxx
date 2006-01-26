#ifndef DISPATCHER_HXX
#define DISPATCHER_HXX 1

#include "repro/WorkerThread.hxx"
#include "repro/Worker.hxx"
#include "resip/stack/ApplicationMessage.hxx"
#include "rutil/TimeLimitFifo.hxx"
#include "rutil/Mutex.hxx"
#include "rutil/Lock.hxx"
#include <vector>


class resip::SipStack;

namespace repro
{

class Dispatcher
{


   public:
      Dispatcher(std::auto_ptr<Worker> prototype, 
                  resip::SipStack* stack,
                  int workers=2, 
                  bool startImmediately=true);

      virtual ~Dispatcher();
      
      /**
         Posts a message to this thread bank.
         
         @param work The message that conveys the work that needs to be done.
            (Any information that must 

         @returns true iff this message was successfully posted. (This may not
            be the case if this Dispatcher is in the process of shutting down)
      */
      virtual bool post(std::auto_ptr<resip::ApplicationMessage> work);

      size_t fifoCountDepth() const;
      time_t fifoTimeDepth() const;
      int workPoolSize() const;
      void shutdownAll();
      void startAll();

      resip::SipStack* mStack;

      
   protected:



      resip::TimeLimitFifo<resip::ApplicationMessage> mFifo;
      bool mAcceptingWork;
      bool mShutdown;
      bool mStarted;
      Worker* mWorkerPrototype;

      resip::Mutex mMutex;

      std::vector<WorkerThread*> mWorkerThreads;


   private:
      //No copying!
      Dispatcher(const Dispatcher& toCopy);
      Dispatcher& operator=(const Dispatcher& toCopy);
};

}

#endif

/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */
