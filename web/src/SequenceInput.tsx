import "./SequenceInput.css";
import React, { FormEvent } from "react";

export interface SequenceInputProps {
    onSubmit(sequence: string): void;
}

export interface SequenceInputEvent extends FormEvent<HTMLTextAreaElement> {
    target: HTMLTextAreaElement;
}

export interface SequenceSubmitEvent extends FormEvent<HTMLFormElement> {
    target: HTMLFormElement;
}

function SequenceInput({ onSubmit }: SequenceInputProps) {
    const regex = /[^ACTGU]/gi;
    const validateInput = ({ target }: SequenceInputEvent) =>
      target.value = target.value.replace(regex, '').toUpperCase()
    const submit = (event: SequenceSubmitEvent) => {
        event.preventDefault();
        const sequence = (event.target.elements.namedItem("formula") as HTMLTextAreaElement).value;
        return regex.test(sequence) || onSubmit(sequence);
    }
    const checkForEnter = (event: React.KeyboardEvent<HTMLTextAreaElement>) => {
        if (event.key == 'Enter')
            ((event.target as HTMLTextAreaElement).parentElement as HTMLFormElement)
                .dispatchEvent(new Event("submit", { cancelable: true, bubbles: true  }));
    }
    return (
        <form onSubmit={submit}>
            <textarea name="formula" onInput={validateInput} onKeyDown={checkForEnter} /><br/>
            <input type="submit" value="Translate" />
        </form>
    )
}

export default SequenceInput;
