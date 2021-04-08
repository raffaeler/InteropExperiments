using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ManagedComponent
{
    public sealed class Alarm : IDisposable
    {
        private Timer _timer;

        public event EventHandler<AlarmEventArgs> Elapsed;

        public string SendReceive(string message) => message;


        public void Start(TimeSpan timeout, string data)
        {
            _timer = new Timer(TimerElapsed, data, (int)timeout.TotalMilliseconds, (int)timeout.TotalMilliseconds);
        }

        public void Stop()
        {
            if (_timer == null) return;
            _timer.Change(Timeout.Infinite, Timeout.Infinite);
        }

        void IDisposable.Dispose()
        {
            if (_timer != null)
            {
                _timer.Dispose();
                _timer = null;
            }
        }

        private void TimerElapsed(object state)
        {
            var data = state as string;
            //var args = "Hello";
            var args = new AlarmEventArgs()
            {
                Message = $"Alarm {data}",
                ElapsedDate = DateTimeOffset.Now,
            };

            Elapsed?.Invoke(this, args);
        }
    }

    public sealed class AlarmEventArgs /*: EventArgs*/
    {
        public string Message { get; set; }
        public DateTimeOffset ElapsedDate { get; set; }
    }

}
