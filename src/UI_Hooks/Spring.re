module Time = Revery_Core.Time;
module Spring = Revery_UI.Spring;

let spring =
    (~enabled=true, ~target, ~initialState=?, ~restThreshold=0.1, options) => {
  let initialState =
    switch (initialState) {
    | Some(state) => state
    | None => Spring.create(target, Time.now())
    };

  let%hook (previousState, setPreviousState) = Ref.ref(initialState);

  let isActive =
    enabled
    && (
      !Spring.isAtRest(~restThreshold, previousState)
      || Float.abs(target -. previousState.value) > restThreshold
    );

  let%hook (time, _) = Timer.timer(~active=isActive, ());

  let state = Spring.tick(target, previousState, options, time);
  setPreviousState(state);

  let setImmediately = position =>
    setPreviousState(Spring.setPosition(position, previousState));

  let v = isActive ? state.value : target;

  (v, setImmediately);
};
