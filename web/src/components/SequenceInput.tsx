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
  const regex = /[^ACGU]/gi;
  const validateInput = ({ target }: SequenceInputEvent) =>
    (target.value = target.value
      .replace(/T/gi, "U")
      .replace(regex, "")
      .toUpperCase());
  const handleSubmit = (event: SequenceSubmitEvent) => {
    event.preventDefault();
    const sequence = (
      event.target.elements.namedItem("formula") as HTMLTextAreaElement
    ).value;
    return regex.test(sequence) || onSubmit(sequence);
  };

  const submitForm = (form: HTMLFormElement) =>
    form.dispatchEvent(
      new Event("submit", {
        cancelable: true,
        bubbles: true,
      })
    );

  const checkForEnter = (event: React.KeyboardEvent<HTMLTextAreaElement>) => {
    if (event.key == "Enter")
      submitForm(
        (event.target as HTMLTextAreaElement).parentElement as HTMLFormElement
      );
  };
  return (
    <form onSubmit={handleSubmit} className="sequence-input">
      <textarea
        name="formula"
        onInput={validateInput}
        onKeyDown={checkForEnter}
        className="sequence-input-box"
      />
      <input type="submit" value="Translate" className="sequence-submit" />
    </form>
  );
}

export default SequenceInput;
