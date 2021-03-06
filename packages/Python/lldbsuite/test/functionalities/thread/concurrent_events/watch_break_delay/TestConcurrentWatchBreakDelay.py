from __future__ import print_function

import unittest2

from lldbsuite.test.decorators import *
from lldbsuite.test.concurrent_base import ConcurrentEventsBase
from lldbsuite.test.lldbtest import TestBase


@skipIfWindows
class ConcurrentWatchBreakDelay(ConcurrentEventsBase):

    mydir = ConcurrentEventsBase.compute_mydir(__file__)

    @skipIfFreeBSD  # timing out on buildbot
    @skipIfRemoteDueToDeadlock
    # Atomic sequences are not supported yet for MIPS in LLDB.
    @expectedFailureAll(triple='^mips')
    def test_watch_break_delay(self):
        """Test watchpoint and a (1 second delay) breakpoint in multiple threads."""
        self.build(dictionary=self.getBuildFlags())
        self.do_thread_actions(
            num_delay_breakpoint_threads=1,
            num_watchpoint_threads=1)
