#pragma once

class StateMachine;

class EventData {
public:
  virtual ~EventData() {}
};

class StateBase {
public:
  virtual void InvokeStateAction(StateMachine *sm,
                                 const EventData *data) const = 0;
};

struct StateMapRow {
  const StateBase *const State;
};

template <class SM, class Data, void (SM::*Func)(const Data *)>
class StateAction : public StateBase {
public:
  virtual void InvokeStateAction(StateMachine *sm,
                                 const EventData *data) const {
    // Downcast the state machine and event data to the correct derived type
    SM *derivedSM = static_cast<SM *>(sm);

    // Dynamic cast the data to the correct derived type
    const Data *derivedData = dynamic_cast<const Data *>(data);
    ASSERT_TRUE(derivedData != 0ull);

    // Call the state function
    (derivedSM->*Func)(derivedData);
  }
};

class StateMachine {
public:
  enum { EVENT_IGNORED = 0xFE, CANNOT_HAPPEN };

  StateMachine(char maxStates, char initialState = 0);
  virtual ~StateMachine() {}

  char GetCurrentState() { return m_currentState; }

protected:
  void ExternalEvent(char newState, const EventData *pData = 0ull);
  void InternalEvent(char newState, const EventData *pData = 0ull);

private:
  const char MAX_STATES;
  char m_currentState;
  char m_newState;
  bool m_eventGenerated;
  const EventData *m_pEventData;

  virtual const StateMapRow *GetStateMap() = 0;
  // virtual const StateMapRowEx *GetStateMapEx() = 0;

  void SetCurrentState(char newState) { m_currentState = newState; }

  void StateEngine(void);
  void StateEngine(const StateMapRow *const pStateMap);
  // void StateEngine(const StateMapRowEx *const pStateMapEx);
};
