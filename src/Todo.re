type item = {
    id: int,
    title: string,
    completed: bool
}
type state = {
    items: list(item)
}
type action = 
  | AddItem(string)
  | ToggleItem(int)
let str = React.string;

module TodoItem = {
    [@react.component]
    let make = (~item, ~onToggle) => {
        <div className="item" onClick=((_e) => onToggle())>
            <input 
                type_="checkbox"
                checked=(item.completed)
            />
            (str(item.title))
        </div>
    }
};

let valueFromEvent = (e): string => e->ReactEvent.Form.target##value;
module Input = {
    type state = string;
    [@react.component]
    let make = (~onSubmit) => {
        let (text, setText) = React.useReducer((oldText, newText) => newText, "");
        <input
            value=text
            type_="text"
            placeholder="Write something to do"
            onChange=((e) => setText(valueFromEvent(e)))
            onKeyDown=((e) => 
                if(ReactEvent.Keyboard.key(e) === "Enter") {
                    onSubmit(text)
                    setText("")
                }
            )
        />
    }
};

let lastId = ref(0);
let newItem = (text) => {
        lastId := lastId^ + 1;
    {   
        id: lastId^,
        title: text,
        completed: false
    }
    
};

[@react.component]
let make = () => {
    let ({items}, dispatch) = React.useReducer((state, action) => {
        switch action {
        | AddItem(text) => {items: [newItem(text), ...state.items]}
        | ToggleItem(id) => 
            let items =List.map(
                (item) =>
                    item.id === id ?
                    {...item, completed: !item.completed}:
                    item,
                state.items
            );
            {items: items}
        };
    }, {
        items: [
            {   id: 0,
                title: "Item 1",
                completed: false
            }
        ]
    });
    let numItems = List.length(items);
    <div className="app">
        <div className="title">
            (str("What to do"))
            <Input onSubmit=((text) => dispatch(AddItem(text))) />
        </div>
        <div className="items">
            (
                React.array(Array.of_list(
                    List.map(
                        (item) => 
                        <TodoItem
                            key=(string_of_int(item.id)) 
                            item
                            onToggle=(() => dispatch(ToggleItem(item.id))) 
                        />, 
                        items
                        )
                ))
            )
            /**
             * OR 
             * List.map((item) => <TodoItem item /> ,items) |> Array.of_list |> React.array
             */
        </div>
        <div className="footer">
            (str(string_of_int(numItems) ++ " items"))
        </div>
</div>
}