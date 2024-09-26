
```mermaid
sequenceDiagram
    participant Client
    participant WDStart
    participant WatchdogRun
    participant Scheduler
    participant SignalHandler
    participant SignalHandler2
    participant SendSignal
    participant CheckThreshold
    participant WDStop

    rect rgb(50, 90, 200)
        Note over Client,WDStart: Initialization
        Client->>WDStart: Call WDStart(threshold, interval, argc, argv)
        activate WDStart
        WDStart->>WDStart: Check for existing watchdog
        WDStart->>WDStart: Create semaphore
        WDStart->>WDStart: Set environment variables
        WDStart->>WatchdogRun: Create thread
        activate WatchdogRun
        WDStart->>WDStart: Create watchdog image
        WDStart-->>Client: Return SUCCESS
        deactivate WDStart
    end

    rect rgb(50, 30, 115)
        Note over WatchdogRun,CheckThreshold: Watchdog Thread
        loop While !stop_wd
            WatchdogRun->>Scheduler: Create scheduler
            WatchdogRun->>SignalHandler: Set up SIG_CHECK handler
            WatchdogRun->>SignalHandler2: Set up SIG_STOP handler
            WatchdogRun->>Scheduler: Add SendSignal task
            WatchdogRun->>Scheduler: Add CheckThreshold task
            WatchdogRun->>Scheduler: Run scheduler
            
            rect rgb(50, 140, 200)
                Note over Scheduler,SignalHandler: Signal Sending
                Scheduler->>SendSignal: Execute SendSignal task
                activate SendSignal
                SendSignal->>SignalHandler: Send SIG_CHECK signal
                SendSignal-->>Scheduler: Return interval
                deactivate SendSignal
            end
            
            rect rgb(255, 150, 0)
                Note over Scheduler,CheckThreshold: Threshold Checking
                Scheduler->>CheckThreshold: Execute CheckThreshold task
                activate CheckThreshold
                CheckThreshold->>CheckThreshold: Check signal counter
                alt signal_counter >= threshold
                    rect rgb(255, 0, 0)
                        Note over CheckThreshold: Threshold Exceeded
                        CheckThreshold->>CheckThreshold: Fork and restart process
                    end
                else signal_counter < threshold
                    rect rgb(140, 240, 140)
                        Note over CheckThreshold: Within Threshold
                    end
                end
                CheckThreshold-->>Scheduler: Return interval
                deactivate CheckThreshold
            end
        end
    end

    rect rgb(150, 100, 100)
        Note over Client,WDStop: Termination Process
        Client->>WDStop: Call WDStop()
        activate WDStop
        WDStop->>SignalHandler2: Send SIG_STOP signal
        SignalHandler2->>WatchdogRun: Set stop_wd flag
        WDStop->>WatchdogRun: Join thread
        deactivate WatchdogRun
        WDStop->>WDStop: Clean up resources
        WDStop-->>Client: Return SUCCESS
        deactivate WDStop
    end
