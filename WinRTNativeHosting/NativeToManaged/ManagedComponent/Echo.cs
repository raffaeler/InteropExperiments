﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ManagedComponent
{
    public sealed class Echo
    {
        public string SendReceive(string message) => message;
    }
}
